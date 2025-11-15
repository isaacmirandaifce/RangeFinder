// ===== rangeFinder.h – Interface da classe RangeFinder =====
// SPDX-License-Identifier: MIT
// Refatorado 2025

#ifndef RANGEFINDER_H
#define RANGEFINDER_H

#include <Arduino.h>

class RangeFinder {
public:
    // Construtor
    RangeFinder(int pinRX, int pinTX, int pinENA, HardwareSerial &serial = Serial2);

    // Inicialização
    void begin(uint32_t baud = 9600);

    // Liga e desliga laser
    void laserOn();
    void laserOff();

    // Reset do módulo
    void reset();

    // Medição rápida (single-shot)
    bool quickMeasure();

    // Última medida válida em metros
    float getLastMeters() const;

    // Leitura contínua automática
    void poll();

private:
    // --- Comandos internos ---
    static constexpr uint8_t CMD_LASER_ON[9]  = {0xAA,0x00,0x01,0xBE,0x00,0x01,0x00,0x01,0xC1};
    static constexpr uint8_t CMD_LASER_OFF[9] = {0xAA,0x00,0x01,0xBE,0x00,0x01,0x00,0x00,0xC0};
    static constexpr uint8_t CMD_QUICK[9]     = {0xAA,0x00,0x00,0x22,0x00,0x01,0x00,0x00,0x23};
    static constexpr uint8_t CMD_READ_RES[5]  = {0xAA,0x80,0x00,0x22,0xA2};

    int _pinRX, _pinTX, _pinENA;
    HardwareSerial &_serial;
    float lastMeters = NAN;

    // --- Funções internas ---
    void powerCycle();
    void clearRX();
    bool csumOK(const uint8_t *f, int n);
    uint32_t bcd32(const uint8_t *b);
    bool readMeters(unsigned long ms);
};

#endif
