#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <DHT_U.h>

// --- Configuração do Display ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Botão ---
#define BUTTON_PIN 38  // Botão para trocar de tela

// --- Sensor DHT22 ---
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// --- Variáveis de controle ---
int currentScreen = 0;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200; // Evita trocas rápidas por ruído
unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 2000; // Atualiza leitura a cada 2s

float temperature = NAN;
float humidity = NAN;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Wire.begin(21, 22); // SDA, SCL
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Erro ao inicializar o display!"));
    for(;;);
  }

  dht.begin();

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop() {
  // --- Troca de tela ---
  if (digitalRead(BUTTON_PIN) == LOW && (millis() - lastDebounceTime) > debounceDelay) {
    currentScreen = (currentScreen + 1) % 2; // alterna entre 0 e 1
    lastDebounceTime = millis();
  }

  // --- Atualiza leitura do DHT periodicamente ---
  if (millis() - lastSensorRead >= sensorInterval) {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    lastSensorRead = millis();
  }

  display.clearDisplay();

  if (currentScreen == 0) {
    drawSensorScreen();
  } else {
    drawLoRaScreen();
  }

  display.display();
  delay(100);
}

void drawSensorScreen() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Sensores:"));
  display.setTextSize(1);
  display.setCursor(0, 20);

  if (isnan(temperature)) {
    display.print(F("Temp: NaN"));
  } else {
    display.print(F("Temp: "));
    display.print(temperature, 1);
    display.print(F("C"));
  }

  display.setCursor(0, 45);
  if (isnan(humidity)) {
    display.print(F("Umid: NaN"));
  } else {
    display.print(F("Umid: "));
    display.print(humidity, 0);
    display.print(F("%"));
  }

  drawIndicators(0);
}

void drawLoRaScreen() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("LoRaWAN Status:"));
  display.setCursor(0, 20);
  display.print(F("Joined"));
  display.setCursor(0, 35);
  display.print(F("RSSI: -78 dBm"));
  display.setCursor(0, 50);
  display.print(F("SNR: 7.2 dB"));

  drawIndicators(1);
}

void drawIndicators(int activeScreen) {
  int y = 60;        // posição vertical das bolinhas
  int xStart = 105;  // posição horizontal inicial
  int spacing = 8;   // espaçamento entre bolinhas
  int radius = 3;    // tamanho da bolinha

  for (int i = 0; i < 2; i++) { // temos 2 telas
    int x = xStart + i * spacing;
    if (i == activeScreen) {
      display.fillCircle(x, y, radius, SSD1306_WHITE); // bolinha cheia
    } else {
      display.drawCircle(x, y, radius, SSD1306_WHITE); // bolinha vazia
    }
  }
}
