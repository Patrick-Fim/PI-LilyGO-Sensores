#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include "LoRaBoards.h"

extern ScreenManager screenManager;

// Região e chip do rádio (Brasil = AU915, SX1276)
#define CFG_au915 1
#define CFG_sx1276_radio 1

// --- Chaves LoRaWAN (substituir pelos valores reais do ChirpStack)
static const u1_t PROGMEM APPEUI[8] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
static const u1_t PROGMEM DEVEUI[8] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
static const u1_t PROGMEM APPKEY[16] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

// --- Funções exigidas pela LMIC ---
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8); }
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8); }
void os_getDevKey (u1_t* buf) { memcpy_P(buf, APPKEY, 16); }

// --- Variáveis ---
bool join_success = false;
uint8_t tx_payload[21];
static osjob_t sendjob;

// --- Mapeamento de pinos LoRa do T-Beam ---
const lmic_pinmap lmic_pins = {
  .nss = RADIO_CS_PIN,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = RADIO_RST_PIN,
  .dio = { RADIO_DIO0_PIN, RADIO_DIO1_PIN, RADIO_DIO2_PIN }
};

// --- Inicialização LoRa ---
void setupLoRa() {
  memset(tx_payload, 0, sizeof(tx_payload));
  Serial.println("Iniciando LoRa...");
  os_init();
  LMIC_reset();
}

// --- Utilitário de impressão ---
void printHex2(uint8_t v) {
  if (v < 16) Serial.print('0');
  Serial.print(v, HEX);
}

// --- Envio de pacote LoRaWAN ---
void do_send(osjob_t* j) {
  if (LMIC.opmode & OP_TXRXPEND) {
    Serial.println(F("OP_TXRXPEND, not sending"));
    return;
  }

  // Atualiza o payload com os valores mais recentes do DHT
  preparePayload();

  Serial.print("Payload: ");
  for (size_t i = 0; i < sizeof(tx_payload); i++) {
    printHex2(tx_payload[i]);
    Serial.print(" ");
  }
  Serial.println();

  LMIC_setTxData2(1, tx_payload, sizeof(tx_payload), 0);
  Serial.println(F("Packet queued"));
}


// --- Callback Informativo ---
void onEvent(ev_t ev) {
  switch (ev) {
    case EV_JOINING:
      Serial.println(F("EV_JOINING - Iniciando conexão OTAA..."));
      break;

    case EV_JOINED:
      Serial.println(F("EV_JOINED - Dispositivo conectado à rede!"));
      join_success = true; // indica que o join deu certo
      screenManager.updateLoRaStatus(true);  // atualiza o display
      LMIC_setLinkCheckMode(0);      // opcional
      Serial.print(F("DevAddr: "));
      Serial.println(LMIC.devaddr, HEX);
      break;

    case EV_JOIN_FAILED:

    case EV_REJOIN_FAILED:

    case EV_JOIN_TXCOMPLETE:
      Serial.println(F("EV_JOIN_TXCOMPLETE - Falha ao conectar, tentando novamente..."));
      break;

    case EV_TXSTART:
      Serial.println(F("EV_TXSTART - Enviando pacote..."));
      break;

    case EV_TXCOMPLETE:
      Serial.println(F("EV_TXCOMPLETE - Pacote enviado com sucesso."));
      if (LMIC.txrxFlags & TXRX_ACK)
        Serial.println(F("  → ACK recebido"));
      if (LMIC.dataLen) {
        Serial.print(F("  → Recebidos "));
        Serial.print(LMIC.dataLen);
        Serial.println(F(" bytes de payload"));
      }
      // Agenda próximo envio
      os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
      break;

    default:
      // Ignora outros eventos
      break;
  }
}
