#ifndef LORACOMUNICACAO_H
#define LORACOMUNICACAO_H

#include <Arduino.h>
#include <LoRa.h>

class LoRaComunicacao {
public:
    // Construtor - recebe a frequência como parâmetro (868E6 ou 915E6)
    LoRaComunicacao(long frequency);

    // Inicializa o LoRa na frequência configurada
    bool iniciar();

    // Envia uma mensagem de texto
    void enviarMensagem(const String& mensagem);

    // Verifica se chegou algum pacote e retorna o conteúdo
    String receberMensagem();

private:
    long _frequencia; // Frequência usada pelo módulo LoRa
};

#endif
