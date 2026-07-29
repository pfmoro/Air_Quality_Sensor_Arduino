// sensors.h
#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

// Protótipos das funções
void initSensors();
void readSensors();
float getCurrentRZero();
void setCurrentRZero(float rzero);

// Declaração de variáveis globais compartilhadas (extern)
extern bool dht_ok;
extern bool mq135_ok;
extern bool oled_ok;

extern float temperatura;
extern float umidade;

extern int mq135_raw_adc;
extern int mq135_raw_corrigido_adc;

extern float ppm_corrigido;
extern float rzero_estimado;

#endif // SENSORS_H