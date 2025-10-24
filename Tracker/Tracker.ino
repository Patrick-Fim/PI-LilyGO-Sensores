#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ScreenManager.h"
#include <DHT.h>
//#include "gps.h"
#include "lorawan.h"

//Sensor Temperatura Umidade
#include <DHT.h>
#define DHT_SENSOR_PIN 2 // ESP32 pin GIOP25 connected to DHT11 sensor
#define DHT_SENSOR_TYPE DHT22

DHT dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

//Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ScreenManager screenManager(&display, 4);

unsigned long lastSerialUpdate = 0;

#define LMIC_DEBUG_LEVEL 2

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

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting");

  // GPS
  //Serial1.begin(9600, SERIAL_8N1, SERIAL1_RX, SERIAL1_TX);

  //SPI pin mapping ESP32 (LILYGO Board V1.1)
  SPI.begin(5, 19, 27, 18);
  //initBoard();

  // Start temp sensor
  dht.begin();
  
  //Set FS for RX2 (SF9 = TTN,TTS EU)
  LMIC.dn2Dr = DR_SF9;

  Serial.println("LMIC");
  Serial.println(CFG_au915);
  
  //beginPower();
  //beginDisplay();

  // Start GPS
  //setupGPS();
  //delay(3000);

  // Start LoRa --> os_init() --> LMIC_reset()
  setupLoRa();
  Serial.println("setupLoRa");
  // Start job (sending automatically starts OTAA too)
  do_send(&sendjob);
  Serial.println("sendjob");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED não detectado, apenas Serial Monitor ativo.");
  }

  screenManager.addScreen(screenLogo);
  screenManager.addScreen(screenTemperature);
  screenManager.addScreen(screenHumidity);
  screenManager.addScreen(screenLoRaWAN);

  screenManager.begin();
}

void loop()
{
  os_runloop_once(); 
  screenManager.update();
}
