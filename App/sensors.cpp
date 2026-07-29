#include "sensors.h"

#include <DHT.h>
#include <Wire.h>
#include "parameters.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include "MQ135_corr.h"
#include "parameters.h"

// ===== Configurações =====
#define DHTTYPE DHT11

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

// ===== Objetos =====
DHT dht(PIN_DHT, DHTTYPE);

MQ135_corr mq135;

// ===== Variáveis Globais =====
float temperatura = NAN;
float umidade = NAN;

int mq135_raw_adc = 0;
int mq135_raw_corrigido_adc = 0;

float ppm_corrigido = NAN;
float rzero_estimado = NAN;

// ===== Estados =====
bool dht_ok = false;
bool mq135_ok = false;
bool oled_ok = false;



// =====================================================
// Inicialização
// =====================================================

void initSensors() {

    dht.begin();

    Serial.println(F("[DHT] Inicializado."));

    
}

// =====================================================
// Leitura Sensores
// =====================================================

void readSensors() {
    // -------------------------------
    // DHT11 / DHT22
    // -------------------------------
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)) {
        dht_ok = false;
        Serial.println(F("[DHT] Falha na leitura."));
    } else {
        temperatura = t;
        umidade = h;
        dht_ok = true;
    }

    // -------------------------------
    // MQ135
    // -------------------------------
    mq135_raw_adc = analogRead(PIN_MQ135);

    // Aplicação da trava e do fator de correção vindo de parameters.h
    mq135_raw_corrigido_adc = constrain(
        round(mq135_raw_adc * correction_factor),
        0,
        1023
    );

    if (mq135_raw_adc <= 0 || mq135_raw_adc >= 1023) {
        mq135_ok = false;
        ppm_corrigido = NAN;
        rzero_estimado = NAN;

        Serial.println(F("[MQ135] Leitura invalida."));
    } else {
        mq135_ok = true;

        if (dht_ok) {
            ppm_corrigido = mq135.getCorrectedPPM(
                mq135_raw_corrigido_adc,
                temperatura,
                umidade
            );

            rzero_estimado = mq135.getRZero(
                mq135_raw_corrigido_adc,
                temperatura,
                umidade
            );
        } else {
            // Continua funcionando, porém sem compensação de temp/umidade
            ppm_corrigido = mq135.getPPM(
                mq135_raw_corrigido_adc
            );

            rzero_estimado = NAN;
        }
    }
    // -------------------------------
    // Umidade Solo
    // -------------------------------
    readSoilMoisture();
}


float getCurrentRZero() {
    return mq135.getRZeroValue();
}

void setCurrentRZero(float rzero) {
    mq135.setRZero(rzero);
}

// --- Em sensors.cpp ---
int soil_raw_adc = 0;
float soil_percent = 0.0f;

void readSoilMoisture() {
    soil_raw_adc = analogRead(PIN_SOIL_MOISTURE);

    // Mapeia o valor bruto para 0-100% invertendo a escala
    // (valores mais altos = mais seco, valores mais baixos = mais úmido)
    float mapped = map(soil_raw_adc, soil_moisture_dry, soil_moisture_wet, 0, 100);
    
    // Trava entre 0 e 100% para evitar valores negativos ou acima de 100
    soil_percent = constrain(mapped, 0.0f, 100.0f);
}

void setSoilCalibration(int dry, int wet) {
    soil_moisture_dry = dry;
    soil_moisture_wet = wet;
}
