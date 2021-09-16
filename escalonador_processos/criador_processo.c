#include <stdlib.h>
#include <stdio.h>

#include "criador_processo.h"
#include "io.h"
#include "valores.h"

cr_processo_t* inicializa_cr_processo(void){
    cr_processo_t* cr_processo = malloc(sizeof(cr_processo_t));
    cr_processo->ocupacaoReserva = 0;

    cr_processo->pidControl = 1;
    cr_processo->quantidadeProcessosAtuais = 0;
    cr_processo->quantidadeProcessosCriados = 0;

    return cr_processo;
}

void deleta_cr_processo(cr_processo_t* cr){
    free(cr);
}

processo_t* cria_processo(unsigned int pidControl){
    unsigned int tempoAtivacao = rand() % (MAX_TEMPO_ATIVACAO);
    unsigned int tempoFinalizacao = rand() % (MAX_TEMPO_EXECUCAO) + 1;

    //Testa se vai ter I/O
    unsigned int n = rand()%5;
    io_t* io_ = NULL;

    switch(n){
        case 2:
            io_ = inicializa_io(rand() % (MAX_TEMPO_IO_ATIVACAO) + 1, rand() % (MAX_TEMPO_IO_EXECUCAO) + 2, DISCO);
            break;
        case 3:
            io_ = inicializa_io(rand() % (MAX_TEMPO_IO_ATIVACAO) + 1, rand() % (MAX_TEMPO_IO_EXECUCAO) + 2, FITA_MAGNETICA);
            break;
        case 4:
            io_ = inicializa_io(rand() % (MAX_TEMPO_IO_ATIVACAO) + 1, rand() % (MAX_TEMPO_IO_EXECUCAO) + 2, IMPRESSORA);
            break;
        default:
            io_ == NULL;
            break;
    }

    return inicializa_processo(pidControl, tempoAtivacao, tempoFinalizacao, io_);
}

void incrementa_controle_pid(cr_processo_t* cr_processo){
    cr_processo->pidControl++;
}

void incrementa_quantidadeProcessosCriados(cr_processo_t* cr_processo){
    cr_processo->quantidadeProcessosCriados++;
}

void incrementa_quantidadeProcessosAtuais(cr_processo_t* cr_processo){
    cr_processo->quantidadeProcessosAtuais++;
}
void decrementa_quantidadeProcessosAtuais(cr_processo_t* cr_processo){
    cr_processo->quantidadeProcessosAtuais--;
}

int checa_limite_max(cr_processo_t* cr){
    return LIMITE_PROCESSOS_MAX >= cr->quantidadeProcessosCriados ? 1 : 0;
}

int checa_limite_atual(cr_processo_t* cr){
    return LIMITE_PROCESSOS_SIMULTANEOS >= cr->quantidadeProcessosAtuais ? 1 : 0;
}

int checa_limite_reserva(cr_processo_t* cr){
    return LIMITE_RESERVA >= cr->ocupacaoReserva;
}
void coloca_reserva(cr_processo_t* cr, processo_t* p){
    cr->reserva[cr->ocupacaoReserva] = p;
}

void print_reserva(cr_processo_t* cr_processo){
    printf("\nRESERVA:\n");
    for(int i = 0; i < cr_processo->ocupacaoReserva; i++){
        print_processo(cr_processo->reserva[i]);
    }
}