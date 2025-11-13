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
- Leitura de sensores de temperatura e umidade no pino 25 do ESP32.
- Exibição opcional de dados no display OLED.
- Envio de dados via LoRa para um servidor Cloud através de um Gateway.
- Monitoramento via Serial Monitor para debug.
- LED Vermelho ao conectar-se com o gateway.

# Como utilizar:
- Necessário instalar o Software [Arduino IDE](https://www.arduino.cc/en/software/). Após instalação, colocar as bibliotecas para o software, normalmente o local é através de: **Documentos\Arduino\libraries**.
- Inicie o Arduino IDE e abra o projeto _PI_ESP.ino_, na pasta de mesmo nome.
- A estrutura do código está preparada tanto para ESP com display quanto sem display, o que será necessário alterar é as linhas de comando no LoraWAN.h, onde terá chaves DEVEUI e APPKEY para cada ESP devidamente numerada, onde deve "Descomentar" (//) as chaves do ESP que será gravado. **Não descomente chaves de dois ou mais ESPs antes de gravar, isso resultará em um erro na gravação**
- Para gravar no ESP, conecte o mesmo via cabo no computador, e na parte superior você seleciona a Board onde será instalado o firmware. Será necessário fazer algumas configurações no Board, através da janela superior:
Tools --> Board --> esp32 --> ESP32 Dev Module
- Colocar upload (seta) e aguardar.
