#ifndef DHTCOMUNICACAO_H
#define DHTCOMUNICACAO_H

#include <DHT.h>

class DHTComunicacao {
  public:
    DHTComunicacao(uint8_t pin, uint8_t type);  

    void iniciar();

    float lerTemperatura();
    
    float lerUmidade();

  private:
    uint8_t pino; // Número do pino digital do ESP32 onde o sensor DHT está conectado
    
    uint8_t tipo; // Tipo do sensor DHT (DHT11, DHT22)

    DHT dht; // Objeto da biblioteca DHT
};

#endif
