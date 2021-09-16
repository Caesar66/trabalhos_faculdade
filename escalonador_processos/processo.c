#include <stdlib.h>
#include <stdio.h>

#include "io.h"
#include "valores.h"
#include "processo.h"

processo_t* inicializa_processo(unsigned int pidControl, unsigned int tempoChegada, 
unsigned int tempoFinalizacao, io_t* io){
    processo_t* processo = malloc(sizeof(processo_t)); 

    processo->pid = pidControl;
    processo->estado = NOVO;
    processo->priori = ALTA;

    processo->tempoChegada = tempoChegada;
    processo->tempoFinalizacao = tempoFinalizacao;
    processo->tempoEmExecucao = 0;
    processo->tempoEmEspera = 0;
    processo->tempoDeVida = 0;

    processo->io = io;
    printf("\nProcesso %d criado.\n", processo->pid);
    return processo;
}

void deleta_processo(processo_t* processo){
    printf("Processo %d finalizado. Tempo Ativo: %d .\n", processo->pid, processo->tempoDeVida);
    deleta_io(processo->io);
    free(processo);
}

const char* pega_estado(processo_t* processo){
    switch(processo->estado){
        case NOVO:
            return "NOVO";
        case PRONTO:
            return "PRONTO";
        case BLOQUEADO:
            return "BLOQUEADO";
        case EXECUCAO:
            return "EXECUCAO";
        case SAIDA:
            return "SAIDA";
    }
    return "NONE";
}

void print_processo(processo_t* processo){
    const char* estado = pega_estado(processo);
    if(processo->io == NULL)
        printf("Processo %d | Estado: %s | T.Ativo: %d | T.Em Execucao: %d | T.Espera (B.P.): %d | T.Chegada: %d | T.de Termino: %d |\n", processo->pid, estado, processo->tempoDeVida, processo->tempoEmExecucao, processo->tempoEmEspera, processo->tempoChegada, processo->tempoFinalizacao);
    else
        printf("Processo %d | Estado: %s | T.Ativo: %d | T.Em Execucao: %d | T.Espera (B.P.): %d | T.Chegada: %d | T.de Termino: %d | T.Chegada I/O: %d | T.Execucao I/O: %d | T.Termino I/O: %d |  Tipo: %s \n", processo->pid, estado, processo->tempoDeVida, processo->tempoEmExecucao, processo->tempoEmEspera, processo->tempoChegada, processo->tempoFinalizacao, processo->io->tempoChegada, processo->io->quantum_io, processo->io->tempoFinalizacao, pega_tipo_io(processo->io));
    
}

void muda_prioridade(processo_t* processo, PRIORIDADE p){
    processo->priori = p;
}
void muda_estado(processo_t* processo, STATUS s){
    processo->estado = s;
}

void incrementa_tempoEmExecucao(processo_t* processo){
    processo->tempoEmExecucao++;
}

void incrementa_tempoEmEspera(processo_t* processo){
    processo->tempoEmEspera++;
}

void incrementa_tempoDeVida(processo_t* processo){
    processo->tempoDeVida++;
}

int checa_fim_execucao(processo_t* processo){
    return processo->tempoFinalizacao == processo->tempoEmExecucao;
}
int checa_lim_tempoEmEspera(processo_t* processo){
    return MAX_TEMPO_ESPERA > processo->tempoEmEspera;
}

io_t* pega_io(processo_t* processo){
    return processo->io;
}