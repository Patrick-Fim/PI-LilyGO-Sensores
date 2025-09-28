#include "LoRaComunicacao.h"

LoRaComunicacao::LoRaComunicacao() {}

void LoRaComunicacao::iniciar(const u1_t* deveui, const u1_t* appeui, const u1_t* appkey) {
    // Inicialização do LMIC
    os_init();
    LMIC_reset();

    // Copia as credenciais
    memcpy_P(DEV_EUI, deveui, 8);
    memcpy_P(APP_EUI, appeui, 8);
    memcpy_P(APP_KEY, appkey, 16);

    // Inicia processo de join OTAA
    LMIC_startJoining();
}

void LoRaComunicacao::enviarDados(uint8_t* dados, uint8_t tamanho) {
    // Envia um pacote uplink
    LMIC_setTxData2(1, dados, tamanho, 0);
    Serial.println(F("Pacote enfileirado para envio"));
}

void LoRaComunicacao::processar() {
    os_runloop_once(); // roda a stack
}
