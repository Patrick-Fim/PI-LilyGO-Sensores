#include <SPI.h>
#include <LoRa.h>

#define LORA_CS   18
#define LORA_RST  23
#define LORA_DIO0 26

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Iniciando Receptor LoRa...");

  SPI.begin(5, 19, 27, LORA_CS);
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(915E6)) {
    Serial.println("Falha ao iniciar LoRa!");
    while (1);
  }

  Serial.println("LoRa iniciado com sucesso!");
  Serial.println("Aguardando pacotes...\n");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize == 0) return; // nada recebido

  uint8_t buffer[4];
  int i = 0;

  while (LoRa.available() && i < 4) {
    buffer[i++] = LoRa.read();
  }

  if (i == 4) {
    uint8_t id = buffer[0];
    int16_t temp_int = (buffer[1] << 8) | buffer[2];
    float temp = temp_int / 10.0;
    uint8_t hum = buffer[3];

    Serial.print("Pacote recebido -> ");
    Serial.print("{ID_ESP: ");
    Serial.print(id);
    Serial.print(", temp: ");
    Serial.print(temp, 1);
    Serial.print(", hum: ");
    Serial.print(hum);
    Serial.println("}");
  } else {
    Serial.println("Pacote com tamanho inesperado!");
  }

  Serial.print("RSSI: ");
  Serial.println(LoRa.packetRssi());
}
