#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define BUTTON_PIN 38

class ScreenManager {
  public:
    ScreenManager(Adafruit_SSD1306* display, int totalScreens);
    void begin();
    void update();  // chama dentro do loop()
    void addScreen(void (*screenFunc)(Adafruit_SSD1306* display));
    
  private:
    Adafruit_SSD1306* _display;
    int _totalScreens;
    int _currentScreen;
    void (*_screens[10])(Adafruit_SSD1306* display); // suporte até 10 telas
    bool _lastButtonState;
};

#endif
