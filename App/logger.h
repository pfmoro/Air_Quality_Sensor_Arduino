#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include "parameters.h"
#include "sensors.h"

// Função responsável por formatar e enviar o JSON diretamente via Serial
void sendJSONData();
//Função para escutar comandos via Serial:
void checkSerialCommands();

#endif // LOGGER_H