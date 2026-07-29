
#include "parameters.h"
#include "sensors.h"
#include "logger.h"

void setup() {

    Serial.begin(115200);


    Serial.println();
    Serial.println(F("================================="));
    Serial.println(F("MQ135 Air Monitor"));
    Serial.println(F("Inicializando..."));
    Serial.println(F("================================="));

   
    initSensors();



    Serial.println(F("Sistema pronto."));
}

unsigned long lastSensorRead = 0;

void loop() {

    unsigned long now = millis();
    checkSerialCommands(); //Escuta comandos do serial
    if (now - lastSensorRead >= INTERVALO_SENSORES) {

        lastSensorRead = now;

        readSensors();
        sendJSONData();
    }

   
    yield();
}