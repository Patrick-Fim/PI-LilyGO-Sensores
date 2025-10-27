#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define BUTTON_PIN 38

class ScreenManager {
  public:
    ScreenManager(Adafruit_SSD1306* display, DHT* dht, int totalScreens);
    void begin();
    void update();  // chama dentro do loop()
    void updateLoRaStatus(bool joined); // para atualizar a tela LoRaWAN

  private:
    Adafruit_SSD1306* _display;
    DHT* _dht;
    int _totalScreens;
    int _currentScreen;
    bool _lastButtonState;
    bool _loraJoined;

    // --- Telas internas ---
    void screenLogo();
    void screenTemperature();
    void screenHumidity();
    void screenLoRaWAN();
    void drawIndicator(uint8_t index, uint8_t total);
};

#endif
