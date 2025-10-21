                    
#define CFG_us915 1
#define CFG_sx1276_radio 1
#define LMIC_DEBUG_LEVEL 2
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// --- CONFIGURAÇÃO DO DISPLAY ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool hasDisplay = false; // detecta se o display existe

// --- BOTÃO ---
#define BUTTON_PIN 38

// --- SENSOR DHT22 ---
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// --- VARIÁVEIS DE CONTROLE ---
int currentScreen = 0;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;
unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 2000; // 2 segundos entre leituras

float temperature = NAN;
float humidity = NAN;

// --- Variáveis para animações ---
int sensorDotCount = 1;
unsigned long lastSensorDotUpdate = 0;
const unsigned long sensorDotInterval = 500;
int dotCount = 1;
unsigned long lastDotUpdate = 0;
const unsigned long dotInterval = 500;

// --- LMIC (LoRaWAN) ---

// APPEUI: Identifica a aplicação ou rede a que os dispositivos percentem
static const u1_t PROGMEM APPEUI[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// --- ESP32 (1) ---
// -- DEVEUI para o RAK: 70B3D57ED0123401 (Invertido no ESP32)
static const u1_t PROGMEM DEVEUI[8] = {0x01, 0x34, 0x12, 0xD0, 0x7E, 0xD5, 0xB3, 0x70};
// -- APPKEY para o ESP32: 52DF61992B114F27B0F6A765418E2619
static const u1_t PROGMEM APPKEY[16] = {0x52, 0xDF, 0x61, 0x99, 0x2B, 0x11, 0x4F, 0x27, 0xB0, 0xF6, 0xA7, 0x65, 0x41, 0x8E, 0x26, 0x19};

// --- ESP32 (2) ---
// -- DEVEUI para o RAK: 70B3D57ED0123402 (Invertido no ESP32)
//static const u1_t PROGMEM DEVEUI[8] = {0x02, 0x34, 0x12, 0xD0, 0x7E, 0xD5, 0xB3, 0x70};
// -- APPKEY para o ESP32: PRECISA AUTENTICAR
//static const u1_t PROGMEM APPKEY[16] = {0};

// --- ESP32 (3) ---
// -- DEVEUI para o RAK: 70B3D57ED0123403 (Invertido no ESP32)
//static const u1_t PROGMEM DEVEUI[8] = {0x03, 0x34, 0x12, 0xD0, 0x7E, 0xD5, 0xB3, 0x70};
// -- APPKEY para o ESP32: PRECISA AUTENTICAR
//static const u1_t PROGMEM APPKEY[16] = {0};

// --- ESP32 (4) ---
// -- DEVEUI para o RAK: 70B3D57ED0123404 (Invertido no ESP32)
//static const u1_t PROGMEM DEVEUI[8] = {0x04, 0x34, 0x12, 0xD0, 0x7E, 0xD5, 0xB3, 0x70};
// -- APPKEY para o ESP32: PRECISA AUTENTICAR
//static const u1_t PROGMEM APPKEY[16] = {0};

void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8); }
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8); }
void os_getDevKey (u1_t* buf) { memcpy_P(buf, APPKEY, 16); }

const lmic_pinmap lmic_pins = {
  .nss = 18,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 23,
  .dio = {26, 33, 32},
};

osjob_t sendjob;
bool joined = false;

// --- FUNÇÃO DE ENVIO DE PACOTES ---
void do_send(osjob_t* j) {
  if (!joined) return;

  float t = isnan(temperature) ? 0 : temperature;
  float h = isnan(humidity) ? 0 : humidity;

  char payload[32];
  int len = snprintf(payload, sizeof(payload), "{\"temp\":%.1f,\"hum\":%.1f}", t, h);

  LMIC_setTxData2(1, (uint8_t*)payload, len, 0);

  Serial.print(F("[LoRa] Pacote agendado -> "));
  Serial.println(payload);
}

// --- EVENTOS LMIC ---
void onEvent(ev_t ev) {
  switch(ev) {
    case EV_JOINING: 
      Serial.println(F("[LoRa] Tentando associar à rede LoRaWAN...")); 
      break;

    case EV_JOINED: 
      Serial.println(F("[LoRa] Associado à rede LoRaWAN!"));
      joined = true; 
      LMIC_setLinkCheckMode(0);
      do_send(&sendjob);
      break;

    case EV_TXCOMPLETE: 
      Serial.println(F("[LoRa] Envio concluído! Novo pacote em 60s.")); 
      os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(10), do_send);
      break;

    default: break;
  }
}

// --- DESENHO OLED ---
void drawIndicators(int activeScreen) {
  int y = 60;
  int xStart = 105;
  int spacing = 8;
  int radius = 3;
  for (int i = 0; i < 2; i++) {
    int x = xStart + i * spacing;
    if (i == activeScreen)
      display.fillCircle(x, y, radius, SSD1306_WHITE);
    else
      display.drawCircle(x, y, radius, SSD1306_WHITE);
  }
}

void drawSensorScreen() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("Dados sensores"));
  for (int i = 0; i < sensorDotCount; i++) display.print(".");

  display.setCursor(0, 20);
  if (isnan(temperature)) display.println(F("Temp: ---"));
  else {
    display.print(F("Temp: "));
    display.print(temperature, 1);
    display.println(F("C"));
  }

  display.setCursor(0, 40);
  if (isnan(humidity)) display.println(F("Umid: ---"));
  else {
    display.print(F("Umid: "));
    display.print(humidity, 1);
    display.println(F("%"));
  }

  drawIndicators(0);
}

void drawLoRaScreen() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("LoRaWAN Status:"));
  display.setCursor(0, 20);
  if (joined) display.print(F("Conectado"));
  else {
    display.print(F("Conectando"));
    for (int i = 0; i < dotCount; i++) display.print(".");
  }
  display.setCursor(0, 40);
  display.print(F("Canal: 915 MHz"));
  drawIndicators(1);
}

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Wire.begin(21, 22); // SDA, SCL

  // tenta inicializar o display
  if (display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    hasDisplay = true;
    display.clearDisplay();
    display.display();
    Serial.println(F("[Display] Detectado com sucesso."));
  } else {
    Serial.println(F("[Display] Não encontrado. Continuando sem ele."));
  }

  dht.begin();
  SPI.begin(5, 19, 27, 18);
  os_init();
  LMIC_reset();
  LMIC_selectSubBand(1);
  LMIC_startJoining();

  Serial.println(F("[Sistema] Iniciando LoRaWAN..."));
}

// --- LOOP ---
void loop() {
  os_runloop_once();

  // botão para trocar de tela
  if (digitalRead(BUTTON_PIN) == LOW && (millis() - lastDebounceTime) > debounceDelay) {
    currentScreen = (currentScreen + 1) % 2;
    lastDebounceTime = millis();
  }

  // animação conectando
  if (!joined && (millis() - lastDotUpdate >= dotInterval)) {
    dotCount++;
    if (dotCount > 3) dotCount = 1;
    lastDotUpdate = millis();
  }

  // leitura de sensores
  if (millis() - lastSensorRead >= sensorInterval) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    temperature = isnan(t) ? NAN : t;
    humidity = isnan(h) ? NAN : h;

    // Mostra no serial mesmo sem display
    Serial.print(F("[Sensor] Temp: "));
    if (isnan(temperature)) Serial.print("---");
    else Serial.print(temperature, 1);

    Serial.print(F(" °C | Umid: "));
    if (isnan(humidity)) Serial.print("---");
    else Serial.print(humidity, 1);
    Serial.println(F(" %"));

    lastSensorRead = millis();
  }

  // animação de leitura de sensor
  if ((isnan(temperature) || isnan(humidity)) && (millis() - lastSensorDotUpdate >= sensorDotInterval)) {
    sensorDotCount++;
    if (sensorDotCount > 3) sensorDotCount = 1;
    lastSensorDotUpdate = millis();
  }

  // renderiza display se existir
  if (hasDisplay) {
    display.clearDisplay();
    if (currentScreen == 0) drawSensorScreen();
    else drawLoRaScreen();
    display.display();
  }
}
