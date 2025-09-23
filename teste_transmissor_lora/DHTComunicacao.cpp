#include "DHTComunicacao.h"

DHTComunicacao::DHTComunicacao(uint8_t pin, uint8_t type) 
  : pino(pin), tipo(type), dht(pin, type) {} // Inicializa os atributos 'pino' e 'tipo' e cria o objeto 'dht' da biblioteca DHT

void DHTComunicacao::iniciar() {
  dht.begin();  // Inicia o sensor
}

float DHTComunicacao::lerTemperatura() {
  return dht.readTemperature(); // Faz a leitura da temperatura e aparece como float. Celsius por padrão
}

float DHTComunicacao::lerUmidade() {
  return dht.readHumidity();  // Faz a leitura da umidade em percentual %
}
