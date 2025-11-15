#include <Arduino.h>
#include "rangeFinder.h"

// ========================================================================
// CONFIGURAÇÃO DOS SENSORES
// Para adicionar mais sensores, basta replicar o bloco abaixo.
// ========================================================================

// SENSOR 1
#define RF1_RX   16
#define RF1_TX   17
#define RF1_ENA   4
RangeFinder laser1(RF1_RX, RF1_TX, RF1_ENA, Serial2);

// SENSOR 2
#define RF2_RX   32
#define RF2_TX   33
#define RF2_ENA  25
RangeFinder laser2(RF2_RX, RF2_TX, RF2_ENA, Serial1);

// SENSOR 3 (opcional)
// #define RF3_RX   26
// #define RF3_TX   27
// #define RF3_ENA  14
// RangeFinder laser3(RF3_RX, RF3_TX, RF3_ENA, Serial2);

// ========================================================================
// PROTÓTIPO DA TASK GENÉRICA
// ========================================================================
void taskRangeFinder(void *param);

// Estrutura simples para repassar parâmetros para cada task
struct RFTaskParam {
    RangeFinder *rf;
    const char *name;
    uint32_t intervalMs;
};

// Cria parâmetros individuais
RFTaskParam p1 = { &laser1, "LASER_1", 1000 };
RFTaskParam p2 = { &laser2, "LASER_2", 1200 };
// RFTaskParam p3 = { &laser3, "LASER_3", 1500 };


// ========================================================================
// SETUP
// ========================================================================
void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("Inicializando sensores...");

    // Inicializa cada módulo
    laser1.begin(9600);
    laser2.begin(9600);
    // laser3.begin(9600);

    // Cria task 1
    xTaskCreatePinnedToCore(
        taskRangeFinder, "taskRF1", 4096, &p1, 1, NULL, 1
    );

    // Cria task 2
    xTaskCreatePinnedToCore(
        taskRangeFinder, "taskRF2", 4096, &p2, 1, NULL, 1
    );

    // Task para sensor 3 (se ativado)
    // xTaskCreatePinnedToCore(
    //     taskRangeFinder, "taskRF3", 4096, &p3, 1, NULL, 1
    // );

    Serial.println("Todas as tasks iniciadas.");
}

void loop() {
    vTaskDelay(portMAX_DELAY);
}


// ========================================================================
// TASK GENÉRICA PARA QUALQUER RANGEFINDER
// ========================================================================
void taskRangeFinder(void *param) {
    RFTaskParam *cfg = (RFTaskParam *)param;
    RangeFinder *rf = cfg->rf;
    const char *name = cfg->name;
    uint32_t intervalMs = cfg->intervalMs;

    for (;;) {
        rf->laserOn();
        vTaskDelay(50 / portTICK_PERIOD_MS);

        bool ok = rf->quickMeasure();

        if (ok) {
            float d = rf->getLastMeters();
            Serial.print(name);
            Serial.print(" => ");
            Serial.print(d, 3);
            Serial.println(" m");
        } else {
            Serial.print(name);
            Serial.println(" => ERRO de leitura");
        }

        rf->laserOff();
        vTaskDelay(intervalMs / portTICK_PERIOD_MS);
    }
}
