#ifndef PROCESSO_H
#define PROCESSO_H

#include "valores.h"
#include "io.h"

typedef struct{
    unsigned int pid;
    io_t* io;
    unsigned int tempoDeVida;
    unsigned int tempoChegada;
    unsigned int tempoFinalizacao;
    unsigned int tempoEmExecucao;
    unsigned int tempoEmEspera;
    STATUS estado;
    PRIORIDADE priori;
}processo_t;

processo_t* inicializa_processo(unsigned int pidControl, unsigned int tempoChegada, 
unsigned int tempoFinalizacao, io_t* io);

void deleta_processo(processo_t* processo);

const char* pega_estado(processo_t* processo);
void print_processo(processo_t* processo);

void incrementa_tempoEmExecucao(processo_t* processo);
void incrementa_tempoDeVida(processo_t* processo);
void incrementa_tempoEmEspera(processo_t* processo);

void muda_prioridade(processo_t* processo, PRIORIDADE p);
void muda_estado(processo_t* processo, STATUS s);

int checa_fim_execucao(processo_t* processo);

int checa_lim_tempoEmEspera(processo_t* processo);

io_t* pega_io(processo_t* processo);
#endif