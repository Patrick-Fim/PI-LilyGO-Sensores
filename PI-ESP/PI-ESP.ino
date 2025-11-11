#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ScreenManager.h"
#include <DHT.h>

// --- Sensor DHT ---
#define DHT_SENSOR_PIN 2
#define DHT_SENSOR_TYPE DHT22
DHT dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// --- Intervalo de envio LoRaWAN ---
const unsigned TX_INTERVAL = 60;  // Pode alterar à vontade

// --- Pacote para enviar via LoRaWAN ---
extern uint8_t tx_payload[21]; // para que preparePayload() consiga acessar

const uint8_t id = 2;

void preparePayload() {

    float temp = dht.readTemperature();

    float hum = dht.readHumidity();

    int16_t temp_int = (int16_t)(temp * 10); // ex: 25,6°C -> 256

    tx_payload[0] = id;
    tx_payload[1] = highByte(temp_int);
    tx_payload[2] = lowByte(temp_int);
    tx_payload[3] = (uint8_t)hum;
}

// --- Pré-Vizualização dos dados ---
unsigned long lastPreview = 0;
const unsigned PREVIEW_INTERVAL = 6000; // 3 segundos

void previewPayload() {
  unsigned long now = millis();
  if (now - lastPreview >= PREVIEW_INTERVAL) {
    lastPreview = now;

    float temp = dht.readTemperature();
    float hum  = dht.readHumidity();

    Serial.print("{ID_ESP: ");
    Serial.print(id);
    Serial.print(", temp: ");
    Serial.print(temp);
    Serial.print(", hum: ");
    Serial.print(hum);
    Serial.println("}");
  }
}

#include "LoRaWAN.h"

// --- Display OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ScreenManager screenManager(&display, &dht, 4); // 4 telas: Logo, Temp, Umidade, LoRa

void setup() {
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  Serial.begin(115200);
  Serial.println(F("Iniciando sistema..."));

  // Inicializa sensor
  dht.begin();

  // Inicializa SPI (LoRa)
  SPI.begin(5, 19, 27, 18);

  // Configura RX2 padrão (TTN/TTS)
  LMIC.dn2Dr = DR_SF9;

  // Inicializa LoRaWAN (faz join automaticamente)
  setupLoRa();

  do_send(&sendjob);

  Serial.println(F("Sistema iniciado com sucesso."));

  // Inicializa Display (Coloque como comentário caso n tenha display)
  /**/
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED não detectado. Continuando apenas com Serial."));
  } else {
    screenManager.begin();  // ScreenManager já desenha as telas
  }
  
}

void loop() {
  os_runloop_once();      // Mantém a pilha LoRaWAN
  screenManager.update(); // Atualiza as telas (Coloque como comentário caso n tenha display)
//  previewPayload();       // Mostra pré-visualização no Serial
}
