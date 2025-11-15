// ===== RangeFinder – Classe OOP para sensor Laser M01 =====
// SPDX-License-Identifier: MIT
// Refatorado 2025

#include <Arduino.h>

class RangeFinder {
public:
    // Construtor
    RangeFinder(int pinRX, int pinTX, int pinENA, HardwareSerial &serial = Serial2)
        : _pinRX(pinRX), _pinTX(pinTX), _pinENA(pinENA), _serial(serial) {}

    // Inicialização
    void begin(uint32_t baud = 9600) {
        pinMode(_pinENA, OUTPUT);
        digitalWrite(_pinENA, HIGH);        // ENA ativo
        _serial.begin(baud, SERIAL_8N1, _pinRX, _pinTX);
        lastMeters = NAN;

        Serial.println("RangeFinder OK: Q=measure L=on K=off R=reset");
    }

    // Liga laser
    void laserOn() {
        _serial.write(CMD_LASER_ON, sizeof(CMD_LASER_ON));
        _serial.flush();
    }

    // Desliga laser
    void laserOff() {
        _serial.write(CMD_LASER_OFF, sizeof(CMD_LASER_OFF));
        _serial.flush();
    }

    // Reset do módulo
    void reset() {
        powerCycle();
        clearRX();
        lastMeters = NAN;
        Serial.println("OK RESET");
    }

    // Medição rápida
    bool quickMeasure() {
        clearRX();
        _serial.write(CMD_QUICK, sizeof(CMD_QUICK));
        _serial.flush();

        if (!readMeters(2000)) {
            _serial.write(CMD_READ_RES, sizeof(CMD_READ_RES));
            _serial.flush();
            return readMeters(800);
        }
        return true;
    }

    // Última medida válida
    float getLastMeters() const {
        return lastMeters;
    }

    // Leitura contínua automática
    void poll() {
        if (_serial.available()) {
            readMeters(200);
        }
    }

private:
    // --- Comandos ---
    static constexpr uint8_t CMD_LASER_ON[9]  = {0xAA,0x00,0x01,0xBE,0x00,0x01,0x00,0x01,0xC1};
    static constexpr uint8_t CMD_LASER_OFF[9] = {0xAA,0x00,0x01,0xBE,0x00,0x01,0x00,0x00,0xC0};
    static constexpr uint8_t CMD_QUICK[9]     = {0xAA,0x00,0x00,0x22,0x00,0x01,0x00,0x00,0x23};
    static constexpr uint8_t CMD_READ_RES[5]  = {0xAA,0x80,0x00,0x22,0xA2};

    int _pinRX, _pinTX, _pinENA;
    HardwareSerial &_serial;
    float lastMeters = NAN;

    // --- Utilidades internas ---
    void powerCycle() {
        digitalWrite(_pinENA, LOW);
        delay(120);
        digitalWrite(_pinENA, HIGH);
        delay(400);
    }

    void clearRX() {
        while (_serial.available()) _serial.read();
    }

    bool csumOK(const uint8_t *f, int n) {
        if (n < 3) return false;
        uint32_t s = 0;
        for (int i = 1; i < n - 1; i++) s += f[i];
        return ((uint8_t)s) == f[n - 1];
    }

    uint32_t bcd32(const uint8_t *b) {
        uint32_t v = 0;
        for (int i = 0; i < 4; i++) {
            v = v * 100 + ((b[i] >> 4) & 0x0F) * 10 + (b[i] & 0x0F);
        }
        return v;
    }

    // Le frames e retorna true se houver medida válida
    bool readMeters(unsigned long ms) {
        uint8_t f[16];
        int pos = 0;
        bool any = false;
        unsigned long t0 = millis(), last = 0;

        while (millis() - t0 < ms) {
            while (_serial.available()) {
                uint8_t x = _serial.read();
                if (pos == 0 && x != 0xAA) continue;

                f[pos++] = x;
                last = millis();

                if (pos == 13) {
                    if (f[0] == 0xAA && f[4] == 0x00 && f[5] == 0x04 && csumOK(f, 13)) {
                        uint8_t func = f[3];
                        if (func == 0x20 || func == 0x21 || func == 0x22) {
                            lastMeters = bcd32(&f[6]) / 1000.0f;
                            Serial.println(lastMeters, 2);
                            any = true;
                        }
                    }
                    pos = 0;
                }
                if (pos >= 13) pos = 0;
            }

            if (pos > 0 && last && millis() - last > 80) {
                pos = 0;
                last = 0;
            }
        }
        return any;
    }
};

