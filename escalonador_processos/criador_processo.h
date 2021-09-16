#ifndef CRIADOR_PROCESSO_H
#define CRIADOR_PROCESSO_H

#include "valores.h"
#include "processo.h"

typedef struct{
    processo_t* reserva[LIMITE_RESERVA];
    unsigned int ocupacaoReserva;

    unsigned int pidControl;
    unsigned int quantidadeProcessosAtuais;
    unsigned int quantidadeProcessosCriados;
    
}cr_processo_t;

cr_processo_t* inicializa_cr_processo(void);
processo_t* cria_processo(unsigned int pidControl);

void incrementa_controle_pid(cr_processo_t* cr_processo);

//Limite de Processos Criados (Usado Para finalizar Loop)
int checa_limite_max(cr_processo_t* cr);

int checa_limite_atual(cr_processo_t* cr);
int checa_limite_reserva(cr_processo_t* cr);

void coloca_reserva(cr_processo_t* cr, processo_t* p);

void incrementa_quantidadeProcessosCriados(cr_processo_t* cr_processo);
void incrementa_quantidadeProcessosAtuais(cr_processo_t* cr_processo);
void decrementa_quantidadeProcessosAtuais(cr_processo_t* cr_processo);

#endif