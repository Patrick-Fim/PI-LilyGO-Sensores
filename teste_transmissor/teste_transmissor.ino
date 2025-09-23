#include "LoRaComunicacao/LoRaComunicacao.h"

// ===== Configurações do LoRa =====
#define LORA_SS 18
#define LORA_RST 14
#define LORA_DIO0 33
long LORA_FREQUENCIA = 868E6; // padrão 868 MHz
LoRaComunicacao lora(LORA_FREQUENCIA);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Inicializa SPI e LoRa
  SPI.begin(5, 19, 27, LORA_SS); // SCK, MISO, MOSI, CS
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!lora.iniciar()) {
    Serial.println("Falha ao iniciar LoRa!");
    while (1);
  }
  Serial.println("LoRa iniciado com sucesso!");
}

void loop() {
  // Envia uma mensagem teste
  String mensagem = "Teste LoRa!";
  lora.enviarMensagem(mensagem);
  Serial.println("Mensagem enviada: " + mensagem);

  // Tenta receber alguma mensagem
  String recebido = lora.receberMensagem();
  if (recebido != "") {
    Serial.println("Mensagem recebida: " + recebido);
  }

  delay(5000); // a cada 5 segundos
}