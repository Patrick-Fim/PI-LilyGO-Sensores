#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ScreenManager.h"
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define DHT_SENSOR_PIN 2
#define DHT_SENSOR_TYPE DHT22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

ScreenManager screenManager(&display, 4);

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
  display->setTextSize(2);
  display->setCursor(10, 10);
  display->println("LoRaWAN:");

  display->setTextSize(1);
  display->setCursor(10, 30);
  display->println("Nome: Nov_Tracker");

  display->setCursor(10, 40);
  display->println("Status: Conectando");

  drawIndicator(display, 2, 3);
  display->display();
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;);
  }
  display.clearDisplay();

  screenManager.addScreen(screenLogo);
  screenManager.addScreen(screenTemperature);
  screenManager.addScreen(screenHumidity);
  screenManager.addScreen(screenLoRaWAN);

  screenManager.begin();
}

void loop() {
  screenManager.update();
}



