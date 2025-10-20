#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ScreenManager.h"
#include <DHT.h>
#include "lorawan.h"

#include <lmic.h>
#include <hal/hal.h>

// --- Pinos do T-Beam SX1276 para LMIC ---
const lmic_pinmap lmic_pins = {
    .nss = 18,               // Chip select (NSS)
    .rxtx = LMIC_UNUSED_PIN, // Não usado
    .rst  = 23,              // Reset do rádio
    .dio  = {26, 33, LMIC_UNUSED_PIN}  // DIO0, DIO1, DIO2
};


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define DHT_SENSOR_PIN 2
#define DHT_SENSOR_TYPE DHT22
#define ESP_NUM 1           // Número do ESP32
#define SERIAL_UPDATE_MS 3000  // intervalo do pacote serial

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

ScreenManager screenManager(&display, 4);

unsigned long lastSerialUpdate = 0;

void drawIndicator(Adafruit_SSD1306* display, int activeScreen, int totalScreens) {
  int radius = 3;
  int spacing = 10;
  int xStart = SCREEN_WIDTH - (totalScreens * radius * 2 + (totalScreens-1) * spacing);
  int y = SCREEN_HEIGHT - 5;

  for (int i = 0; i < totalScreens; i++) {
    int x = xStart + i * (2 * radius + spacing);
    if (i == activeScreen) {
      display->fillCircle(x, y, radius, SSD1306_WHITE);
    } else {
      display->drawCircle(x, y, radius, SSD1306_WHITE);
    }
  }
}

// --- Telas ---
void screenLogo(Adafruit_SSD1306* display) {
  display->setTextSize(2);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(15, 10);
  display->println("PI ESP32");
  display->display();
}

void screenTemperature(Adafruit_SSD1306* display) {
  float temp = dht.readTemperature();
  display->setTextSize(2);
  display->setCursor(10, 10);
  display->println("Temp:");
  display->setCursor(10, 30);
  display->println(String(temp) + " C");
  drawIndicator(display, 0, 3);
  display->display();
}

void screenHumidity(Adafruit_SSD1306* display) {
  float hum = dht.readHumidity();
  display->setTextSize(2);
  display->setCursor(10, 10);
  display->println("Umidade:");
  display->setCursor(10, 30);
  display->println(String(hum) + " %");
  drawIndicator(display, 1, 3);
  display->display();
}

void screenLoRaWAN(Adafruit_SSD1306* display) {
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->println(F("Status LoRaWAN:"));
  display->setCursor(0, 20);
  display->println(F("Conectando..."));
  drawIndicator(display, 2, 3);
  display->display();
}

void setup() {
  SPI.begin(5, 19, 27, 18); // SCK, MISO, MOSI, NSS

  Serial.begin(115200);
  dht.begin();

  // Inicializa display se tiver
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED não detectado, apenas Serial Monitor ativo.");
  }

  // --- DEBUG: Inicialização LoRaWAN ---
Serial.println("Chamando os_init_ex()");
os_init_ex(&lmic_pins);   // inicializa LMIC com seu pinmap
Serial.println("os_init_ex() concluído");
LMIC_reset();
Serial.println("Chamando LMIC_startJoining()");
LMIC_startJoining();      // tenta ingressar na rede
Serial.println("LMIC_startJoining() concluído");


  // Aqui você pode colocar as telas
  screenManager.begin();
}


void loop() {
//  os_runloop_once(); // mantém o LMIC rodando
  screenManager.update(); // atualiza as telas do OLED
}


