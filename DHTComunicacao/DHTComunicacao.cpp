#include "DHTComunicacao.h"

DHTComunicacao::DHTComunicacao(uint8_t pin, uint8_t type) 
  : pino(pin), tipo(type), dht(pin, type) {}

void DHTComunicacao::iniciar() {
  dht.begin();
}

float DHTComunicacao::lerTemperatura() {
  return dht.readTemperature(); // Celsius por padrão
}

float DHTComunicacao::lerUmidade() {
  return dht.readHumidity();
}
