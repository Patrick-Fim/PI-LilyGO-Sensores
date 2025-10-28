# PI-LilyGO-Sensores
Repositório desenvolvido para o Projeto Integrador do quarto semestre de Ciência da Computação (UNIJUÍ). O objetivo é implementar um sistema de coleta de informações utilizando o ESP32 LilyGO, para posteriormente, enviá-los para um servidor para monitoramento e análise.

O objetivo é implementar um sistema de coleta de informações utilizando o ESP32 LilyGO, exibindo dados em um display OLED e enviando-os via LoRa para outro dispositivo ou gateway para monitoramento e análise.

# Ambiente de Desenvolvimento
- IDE: Arduino IDE
- Linguagem: C++ (Arduino Framework)
- Bibliotecas:
  - Adafruit_BusIO
  - Adafruit_GFX_Library
  - Adafruit_Sensor-master
  - Adafruit_SSD1306
  - DHT_sensor_library
  - LoRa
  - MCCI_LoRaWAN_LMIC_library
  - SD
  - U8g2
  - XPowersLib


# Componentes Utilizados
- Microcontrolador: ESP32 LilyGO T-Display
- Sensores: DHT22 (temperatura e umidade)
- Módulo de comunicação: LoRa (868/915 MHz)
- Display: OLED 128x64 (opcional, depende do ESP32 usado)

# Funcionalidades
- Leitura de sensores de temperatura e umidade.
- Exibição opcional de dados no display OLED.
- Envio de dados via LoRa para um gateway.
- Monitoramento via Serial Monitor para debug.
