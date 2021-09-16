#ifndef CONTROLADOR_PROCESSO_H
#define CONTROLADOR_PROCESSO_H

#include "fila.h"
#include "processo.h"
#include "valores.h"
#include "criador_processo.h"


typedef struct{
    unsigned int contadorTempo;
    unsigned int contadorQuantum;

    fila_t* filaAltaPrioridade;
    fila_t* filaBaixaPrioridade;

    fila_t* filaDisco;
    fila_t* filaFita;
    fila_t* filaImpressora;

    p_elemento_t* processoEmExecucao;

    p_elemento_t* IODisco;
    p_elemento_t* IOFita;
    p_elemento_t* IOImpressora;
}controlador_t;

controlador_t* inicializa_control(void);

void deleta_control(controlador_t* ct);

void muda_fila(fila_t* fila_antiga, fila_t* fila_nova, unsigned int pid);

//Essa funcAO coloca processo do inicio da fila de alta prioridade
// para em execucao ou das filas de IO para execucao em IO
void redireciona_processo(controlador_t* ct, fila_t* fila);

p_elemento_t* pega_processo_execucao(controlador_t* ct);

void coloca_elemento_fila_io(controlador_t* ct, p_elemento_t* e);

int checa_filas_vazias(controlador_t* ct);

void executa_processo_emExecucao(controlador_t* control, cr_processo_t* criador);

void checa_fila_altaPrioridade(controlador_t* control);

void checa_fila_baixaPrioridade(controlador_t* control);

void checa_fila_io(controlador_t* control, IO_OPERACAO io_tipo);

void checa_reserva(controlador_t* control, cr_processo_t* criador);

void checa_criacao_processos(controlador_t* control, cr_processo_t* criador);

#endif