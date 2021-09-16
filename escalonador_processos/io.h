#ifndef IO_H
#define IO_H

#include "valores.h"

typedef struct{
    unsigned int quantum_io;

    unsigned int tempoChegada;
    unsigned int tempoFinalizacao;
    IO_OPERACAO tipo;
}io_t;

io_t* inicializa_io(unsigned int tempoChegada, unsigned int tempoFinalizacao, IO_OPERACAO tipo);

void incrementa_quantum_io(io_t* io);

unsigned int checa_tempoChegada_io(unsigned int tempo, io_t* io);
unsigned int checa_fim_io(io_t* io);

void deleta_io(io_t* io);

const char* pega_tipo_io(io_t* io);

#endif