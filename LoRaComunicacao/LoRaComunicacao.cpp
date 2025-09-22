#include "LoRaComunicacao.h"

// Construtor: recebe a frequência desejada
LoRaComunicacao::LoRaComunicacao(long frequency) {
    _frequencia = frequency;
}

// Inicializa o LoRa na frequência configurada
bool LoRaComunicacao::iniciar() {
    if (!LoRa.begin(_frequencia)) {
        Serial.println("Falha ao inicializar LoRa!");
        return false; // falha ao iniciar
    } else {
        Serial.println("LoRa inicializado com sucesso!");
        return true; // iniciado com sucesso
    }
}

// Envia uma mensagem de texto
void LoRaComunicacao::enviarMensagem(const String& mensagem) {
    LoRa.beginPacket();
    LoRa.print(mensagem);
    LoRa.endPacket();
    Serial.println("Enviado a mensagem: " + mensagem);
}

// Verifica se chegou algum pacote e retorna o conteúdo
String LoRaComunicacao::receberMensagem() {
    int packetSize = LoRa.parsePacket();
    String recebido = "";

    if (packetSize) {
        while (LoRa.available()) {
            recebido += (char)LoRa.read();
        }
    }

    return recebido; // vazio se não tiver nada
}
