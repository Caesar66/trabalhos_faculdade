#include <stdlib.h>

#include "io.h"

io_t* inicializa_io(unsigned int tempoChegada, unsigned int tempoFinalizacao, IO_OPERACAO tipo){
    io_t* io = malloc(sizeof(io_t));

    io->quantum_io = 0;
    io->tempoChegada = tempoChegada;
    io->tempoFinalizacao = tempoFinalizacao;

    io->tipo = tipo;

    return io;
}

void incrementa_quantum_io(io_t* io){
    io->quantum_io++;
}

unsigned int checa_fim_io(io_t* io){
    return io->quantum_io < io->tempoFinalizacao ? 1 : 0;
}

unsigned int checa_tempoChegada_io(unsigned int tempo, io_t* io){
    return tempo == io->tempoChegada ? 1 : 0;
}

void deleta_io(io_t* io){
    free(io);
}

const char* pega_tipo_io(io_t* io){
    switch(io->tipo){
        case DISCO:
            return "DISCO";
        case FITA_MAGNETICA:
            return "FITA";
        case IMPRESSORA:
            return "IMPRESSORA";
    }
    return "NULL";
}
