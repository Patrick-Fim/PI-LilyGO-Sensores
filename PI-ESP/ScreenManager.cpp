#include "ScreenManager.h"

ScreenManager::ScreenManager(Adafruit_SSD1306* display, DHT* dht, int totalScreens) {
  _display = display;
  _dht = dht;
  _totalScreens = totalScreens;
  _currentScreen = 0;
  _lastButtonState = HIGH;
  _loraJoined = false;
}

void ScreenManager::begin() {
  pinMode(BUTTON_PIN, INPUT);
}

void ScreenManager::updateLoRaStatus(bool joined) {
  _loraJoined = joined;
}

void ScreenManager::update() {
  bool buttonState = digitalRead(BUTTON_PIN);

  // Detecta borda de descida (botão pressionado)
  if (_lastButtonState == HIGH && buttonState == LOW) {
    _currentScreen = (_currentScreen + 1) % _totalScreens;
  }
  _lastButtonState = buttonState;

  _display->clearDisplay();
  switch (_currentScreen) {
    case 0: screenLogo(); break;
    case 1: screenTemperature(); break;
    case 2: screenHumidity(); break;
    case 3: screenLoRaWAN(); break;
  }
  _display->display();
}

// --- Funções de tela ---
void ScreenManager::screenLogo() {
  _display->setTextSize(2);
  _display->setTextColor(SSD1306_WHITE);
  _display->setCursor(15, 10);
  _display->println(F("PI ESP32"));
  drawIndicator(0, _totalScreens);
}

void ScreenManager::screenTemperature() {
  float temp = _dht->readTemperature();
  _display->setTextSize(2);
  _display->setTextColor(SSD1306_WHITE);
  _display->setCursor(10, 10);
  _display->println(F("Temp:"));
  _display->setCursor(10, 30);
  _display->println(String(temp, 1) + " C");
  drawIndicator(1, _totalScreens);
}

void ScreenManager::screenHumidity() {
  float hum = _dht->readHumidity();
  _display->setTextSize(2);
  _display->setTextColor(SSD1306_WHITE);
  _display->setCursor(10, 10);
  _display->println(F("Umidade:"));
  _display->setCursor(10, 30);
  _display->println(String(hum, 1) + " %");
  drawIndicator(2, _totalScreens);
}

void ScreenManager::screenLoRaWAN() {
  _display->setTextSize(1);
  _display->setTextColor(SSD1306_WHITE);
  _display->setCursor(0, 0);
  _display->println(F("Status LoRaWAN:"));
  _display->setCursor(0, 20);
  _display->println(_loraJoined ? F("Conectado!") : F("Conectando..."));
  drawIndicator(3, _totalScreens);
}

// --- Indicador de tela (pontinhos inferiores) ---
void ScreenManager::drawIndicator(uint8_t index, uint8_t total) {
  int spacing = 10;
  int startX = (128 - (total - 1) * spacing) / 2;
  int y = 60;
  for (uint8_t i = 0; i < total; i++) {
    _display->fillCircle(startX + i * spacing, y, (i == index) ? 3 : 1, SSD1306_WHITE);
  }
}
