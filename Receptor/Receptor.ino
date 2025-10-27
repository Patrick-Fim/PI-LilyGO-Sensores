#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

#define DHT_SENSOR_PIN 2
#define DHT_SENSOR_TYPE DHT22
DHT dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

const int ID_ESP = 1;

#define LORA_CS 18
#define LORA_RST 23
#define LORA_DIO0 26

void setup() {
  Serial.begin(115200);
  dht.begin();

  SPI.begin(5, 19, 27, 18); 
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);

  while (!LoRa.begin(915E6)) {
    Serial.println("Tentando inicializar LoRa...");
    delay(500);
  }
  Serial.println("LoRa OK!");
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  int16_t temp_int = (int16_t)(temp * 10);

  uint8_t payload[4];
  payload[0] = ID_ESP;
  payload[1] = highByte(temp_int);
  payload[2] = lowByte(temp_int);
  payload[3] = (uint8_t)hum;

  LoRa.beginPacket();
  LoRa.write(payload, sizeof(payload));
  LoRa.endPacket();

  Serial.print("Enviado: ");
  Serial.print("ID_ESP: "); Serial.print(ID_ESP);
  Serial.print(" | Temp: "); Serial.print(temp);
  Serial.print(" | Hum: "); Serial.println(hum);

  delay(3000);
}
