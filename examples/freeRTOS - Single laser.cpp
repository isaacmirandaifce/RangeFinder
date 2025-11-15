#include <Arduino.h>
#include "rangeFinder.h"

// Defina os pinos do seu Laser M01
// Exemplo comum no ESP32:
#define PIN_RX   16
#define PIN_TX   17
#define PIN_ENA  4

// Objeto global do sensor
RangeFinder rf(PIN_RX, PIN_TX, PIN_ENA, Serial2);

// Protótipo da task
void taskRangeFinder(void *param);

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("Inicializando RangeFinder...");
    rf.begin(9600);

    // Cria task FreeRTOS
    xTaskCreatePinnedToCore(
        taskRangeFinder,   // função
        "taskRangeFinder", // nome
        4096,              // stack
        NULL,              // parâmetro
        1,                 // prioridade
        NULL,              // handle
        1                  // roda no Core 1
    );

    Serial.println("Task criada.");
}

void loop() {
    // Não usar loop; FreeRTOS gerencia tudo.
    vTaskDelay(portMAX_DELAY);
}

// ====================================================================
// TASK FreeRTOS
// ====================================================================
void taskRangeFinder(void *param) {
    (void)param;

    for (;;) {
        // Liga laser (opcional)
        rf.laserOn();
        vTaskDelay(50 / portTICK_PERIOD_MS);

        // Faz uma medida
        bool ok = rf.quickMeasure();

        if (ok) {
            float d = rf.getLastMeters();
            Serial.print("Distancia: ");
            Serial.print(d, 3);
            Serial.println(" m");
        } else {
            Serial.println("Falha na leitura.");
        }

        // Desliga laser (opcional)
        rf.laserOff();

        // Intervalo entre medidas
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
