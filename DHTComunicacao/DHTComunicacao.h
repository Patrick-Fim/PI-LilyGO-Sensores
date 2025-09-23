#ifndef DHTCOMUNICACAO_H
#define DHTCOMUNICACAO_H

#include <DHT.h>

class DHTComunicacao {
  private:
    uint8_t pino;
    uint8_t tipo;
    DHT dht;

  public:
    DHTComunicacao(uint8_t pin, uint8_t type);

    void iniciar();

    float lerTemperatura();
    
    float lerUmidade();
};

#endif
