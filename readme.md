
---

# Monitoramento de Qualidade do Ar com Arduino (MQ‑135 + DHT11 ) 💨

Placa e firmware dedicados ao monitoramento da qualidade do ar e medição ambiental utilizando os sensores **[MQ-135](https://www.makerhero.com/blog/como-funciona-o-sensor-de-gas-mq-135/?srsltid=AfmBOorwAhi8It0JkbR7EZQjq7tZ1lGu5Q90_x1cF1DfJ-fndEwpIFdH)** e **DHT11**, com exibição local via **OLED**, interface web via **JSON** e telemetria em nuvem.

Esta versão do projeto reflete uma arquitetura modularizada em C++ com suporte para:

* **Fator de correção dinâmico (`correction_factor`)**: suporte para uso transparente em **5V (Arduino)** ou **3.3V (NodeMCU / ESP32 com divisor)**.
* **Módulo de Logging Isolado (`logger`)**: geração e formatação de dados em JSON otimizados para economia de memória RAM.
* **Biblioteca `MQ135_corr**`: fork local com processo de calibração RZero e compensação de umidade e temperatura.
* **Telemetria e Conectividade**: suporte a envio via serial

---

## 🔩 Hardware e Divisor de Tensão

### Sensores e Alimentação

* **MQ‑135**: Sensor de qualidade do ar (CO₂ equivalente / VOCs).


* **DHT11**: Sensor de temperatura e umidade (usado na compensação matemática das leituras químicas).


* **Divisor de Tensão**: Utilizado no pino A0 quando operando em microcontroladores de 3,3V (NodeMCU / ESP32).



> ⚠️ O MQ‑135 **deve ser alimentado em 5V (VIN)** para correto funcionamento do elemento de aquecimento interno.
> 
> 

### Tabela de Pinagem Recomendada

| Componente | Pino Arduino / NodeMCU | Observação |
| --- | --- | --- |
| MQ‑135 (VCC) | **VIN (5V)** | Alimentação obrigatória do aquecedor

 |
| MQ‑135 (GND) | **GND** | Terra comum |
| MQ‑135 (A0) | **A0 (ou via Divisor)** | Pino analógico de leitura |
| DHT11 (VCC) | **5V / 3V3** | Alimentação lógica

 |
| DHT11 (DATA) | **D2 (GPIO4 / Pin 2)** | Entrada digital |
| DHT11 (GND) | **GND** | Terra comum |

---

## 📂 Estrutura Modular do Firmware

O firmware foi estruturado em módulos independentes para facilitar a manutenção e evitar conflitos de compilação:

```text
├── app.ino           # Script principal (setup, loop e orquestração)
├── parameters.h      # Declaração das variáveis de configuração (correction_factor, pinos)
├── parameters.cpp    # Definição e inicialização real das configurações globais
├── sensors.h         # Declaração das funções e estados compartilhados dos sensores
├── sensors.cpp       # Leitura do DHT11, MQ-135, aplicação de ganhos e cálculo de PPM/RZero
├── logger.h          # Protótipos das funções de exportação de dados
└── logger.cpp        # Função sendJSONData() leve e formatada via F()

```

---

## 🚀 Arquitetura de Software

### 1. Parametrização (`parameters.h` e `parameters.cpp`)

Contém o fator de correção de voltagem configurável:

* **`correction_factor = 1.0f`**: Padrão para placas operando em 5V nativos (Arduino Uno/Nano).
* **`correction_factor = 1.5f`**: Para microcontroladores de 3.3V utilizando divisor de tensão de ratio $\approx 0,66$ (NodeMCU/ESP32).



### 2. Aquisição de Dados (`sensors.cpp`)

Responsável pela leitura periódica dos sensores e pela compensação térmica:

* A leitura do ADC bruta do MQ-135 passa por tratamento com `constrain` e pelo `correction_factor`.
* Caso o sensor DHT11 apresente leitura válida (`dht_ok == true`), o cálculo de PPM e RZero estimado utiliza a compensação de temperatura e umidade da biblioteca `MQ135_corr`.



### 3. Módulo de Logging (`logger.cpp`)

A função `sendJSONData()` envia a telemetria via Serial em formato JSON minimizado sem alocar buffers pesados em memória RAM:

```json
{
  "dht_ok": true,
  "temp": 25.4,
  "umid": 61.2,
  "mq135_ok": true,
  "adc_raw": 312,
  "ppm": 11.80,
  "rzero": 76.43
}

```

---

## 🔥 Burn‑In e Calibração (MQ-135)

1. **Burn‑In Mínimo**: Deixe o módulo ligado por no mínimo **20 minutos** antes de considerar leituras válidas.


2. **Calibração Real**: Mantenha o sensor alimentado continuadamente por **≥ 24 horas em ambiente limpo/ventilado**.


3. **Obtenção do RZero**:
Observe o parâmetro `rzero` impresso no JSON do `sendJSONData()` para ajustar a constante padrão da biblioteca `MQ135_corr` ao seu hardware específico.



---

## ✅ Checklist de Verificação

* [ ] MQ‑135 alimentado em **5V (VIN)**.


* [ ] Arquivo `parameters.cpp` configurado com o `correction_factor` correto para o seu hardware (1.0 para Arduino, 1.5 para NodeMCU/ESP32 com divisor).
* [ ] Módulo `sensors` compilando sem avisos de escopo ou redeclaração (`extern`).
* [ ] Burn-in do MQ-135 efetuado previamente.