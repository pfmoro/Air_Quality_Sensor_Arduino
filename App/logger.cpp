#include "logger.h"
#include "parameters.h"



void sendJSONData() {
    // Monta um objeto JSON simples economizando memória RAM
    Serial.print(F("{"));
    
    Serial.print(F("\"dht_ok\":"));
    Serial.print(dht_ok ? F("true") : F("false"));
    Serial.print(F(","));
    
    Serial.print(F("\"temp\":"));
    if (isnan(temperatura)) Serial.print(F("null"));
    else Serial.print(temperatura, 1);
    Serial.print(F(","));

    Serial.print(F("\"umid\":"));
    if (isnan(umidade)) Serial.print(F("null"));
    else Serial.print(umidade, 1);
    Serial.print(F(","));

    Serial.print(F("\"mq135_ok\":"));
    Serial.print(mq135_ok ? F("true") : F("false"));
    Serial.print(F(","));

    Serial.print(F("\"adc_raw\":"));
    Serial.print(mq135_raw_adc);
    Serial.print(F(","));

    Serial.print(F("\"ppm\":"));
    if (isnan(ppm_corrigido)) Serial.print(F("null"));
    else Serial.print(ppm_corrigido, 2);
    Serial.print(F(","));

    Serial.print(F("\"rzero\":"));
    if (isnan(rzero_estimado)) Serial.print(F("null"));
    else Serial.print(rzero_estimado, 2);

    Serial.print(F("\"soil_raw\":"));
    Serial.print(soil_raw_adc);
    Serial.print(F(","));
    
    Serial.print(F("\"soil_percent\":"));
    Serial.print(soil_percent, 1);

    Serial.println(F("}"));
}

void checkSerialCommands() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim(); // Remove espaços e \r\n

        // Comando para alterar o RZero em tempo de execução
        if (input.startsWith("SET_RZERO=")) {
            float new_rzero = input.substring(10).toFloat();
            
            if (new_rzero > 0.0f) {
                setCurrentRZero(new_rzero);
                Serial.print(F("{\"status\":\"ok\",\"msg\":\"RZero atualizado para "));
                Serial.print(new_rzero, 2);
                Serial.println(F("\"}"));
            } else {
                Serial.println(F("{\"status\":\"error\",\"msg\":\"Valor invalido\"}"));
            }
        }
        if (input.startsWith("SET_SOIL=")) {
            String values = input.substring(9);
            int commaIndex = values.indexOf(',');
            
            if (commaIndex > 0) {
                int dry = values.substring(0, commaIndex).toInt();
                int wet = values.substring(commaIndex + 1).toInt();
                
                setSoilCalibration(dry, wet);
                
                Serial.print(F("{\"status\":\"ok\",\"msg\":\"Calibracao solo atualizada. Seco: "));
                Serial.print(dry);
                Serial.print(F(" Molhado: "));
                Serial.print(wet);
                Serial.println(F("\"}"));
            } else {
                Serial.println(F("{\"status\":\"error\",\"msg\":\"Formato invalido. Use SET_SOIL=SECO,MOLHADO\"}"));
            }
        }
    
    }
}