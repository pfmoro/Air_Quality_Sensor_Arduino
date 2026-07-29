#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <Arduino.h>
// =====================================================
// INTERVALOS
// =====================================================

// Aquisição dos sensores
const unsigned long INTERVALO_SENSORES = 5000;

// Declarado como 1.0 por padrão (Arduino / 5V). Altere para 1.5 em 3.3V (NodeMCU / ESP32)
extern float correction_factor;
// =====================================================
// MAPEAMENTO DE PINOS (Arduino Uno / Nano)
// =====================================================
const uint8_t PIN_DHT = 2;       // Antigo D2 do NodeMCU
const uint8_t PIN_MQ135 = A0;    // Pino Analógico A0
const uint8_t PIN_SOIL_MOISTURE = A1; // Pino Analógico A1

// --- Configurações do sensor capacitivo ---
extern const uint8_t PIN_SOIL_MOISTURE;
extern int soil_moisture_dry; // Valor lido no ar (100% seco)
extern int soil_moisture_wet; // Valor lido na água (100% molhado)
// =====================================================
// SEGURANÇA
// =====================================================

// Limite mínimo aceitável para heap livre.
// Atualmente não utilizado, mas mantido como
// ponto de expansão futura.
#define CRITICAL_HEAP_LIMIT 8000

#endif