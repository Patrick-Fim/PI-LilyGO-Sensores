#include "ScreenManager.h"

ScreenManager::ScreenManager(Adafruit_SSD1306* display, int totalScreens) {
  _display = display;
  _totalScreens = totalScreens;
  _currentScreen = 0;
  _lastButtonState = HIGH; // pull-up
}

void ScreenManager::begin() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void ScreenManager::addScreen(void (*screenFunc)(Adafruit_SSD1306* display)) {
  static int index = 0;
  if(index < 10) {
    _screens[index++] = screenFunc;
  }
}

void ScreenManager::update() {
  bool buttonState = digitalRead(BUTTON_PIN);
  // Detecta borda de descida (aperto)
  if(_lastButtonState == HIGH && buttonState == LOW) {
    _currentScreen = (_currentScreen + 1) % _totalScreens;
  }
  _lastButtonState = buttonState;

  _display->clearDisplay();
  _screens[_currentScreen](_display);
  _display->display();
}
