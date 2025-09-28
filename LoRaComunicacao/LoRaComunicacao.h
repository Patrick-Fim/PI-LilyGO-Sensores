#ifndef LORACOMUNICACAO_H
#define LORACOMUNICACAO_H

#include <Arduino.h>
#include <lmic.h> // MCCI LoRaWAN LMIC library
#include <hal/hal.h>

// Classe para encapsular o envio LoRaWAN
class LoRaComunicacao {
public:
    LoRaComunicacao();

    // Inicializa a stack LoRaWAN com as credenciais
    void iniciar(const u1_t* deveui, const u1_t* appeui, const u1_t* appkey);

    // Envia dados (binário)
    void enviarDados(uint8_t* dados, uint8_t tamanho);

    // Deve ser chamado no loop() para o LMIC funcionar
    void processar();
};

#endif
