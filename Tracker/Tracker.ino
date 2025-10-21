// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "gps.h"
#include "lorawan.h"

//Sensor Temperatura Umidade
#include <DHT.h>
#define DHT_SENSOR_PIN  2 // ESP32 pin GIOP25 connected to DHT11 sensor
#define DHT_SENSOR_TYPE DHT22

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

#define BUTTON_PIN                  38

// Variaveis Globais

float LatInt=0;
float LatDec=0;
int LatOri=0;

float LonInt=0;
float LonDec=0;
int LonOri=0;

float Altitude=0;
int Sat=0;
float Temp=0;
float Humi=0;

#define LMIC_DEBUG_LEVEL 2

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting");
  Serial1.begin(9600, SERIAL_8N1, SERIAL1_RX, SERIAL1_TX);
  //SPI pin mapping ESP32 (LILYGO Board V1.1)
  SPI.begin(5, 19, 27, 18);
  //initBoard();

// Start temp sensor
  dht_sensor.begin();
  

  
  // Reset the MAC state. Session and pending data transfers will be discarded.
  //LMIC_reset();

  //Set FS for RX2 (SF9 = TTN,TTS EU)
  LMIC.dn2Dr = DR_SF9;

  Serial.println("LMIC");
  //Serial.println(CFG_au915);
  
  beginPower();
  beginDisplay();

  // Start GPS
  setupGPS();
  delay(3000);

  // Start LoRa --> os_init() --> LMIC_reset()
  setupLoRa();

  // Start job (sending automatically starts OTAA too)
  do_send(&sendjob);
  Serial.println("do_send");  

}

void loop()
{
  os_runloop_once(); 
}
