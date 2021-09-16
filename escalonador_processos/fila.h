#ifndef FILA_H
#define FILA_H

#include "valores.h"
#include "processo.h"

/* Fila de processos utilizando alocação dinamica*/
typedef struct p_elemento_t p_elemento_t;
struct p_elemento_t{
    processo_t* processo;
    p_elemento_t* proximo;
};

typedef struct{
    p_elemento_t* inicio;
    unsigned int tamanho;

    TIPO_FILA tipo;
    PRIORIDADE prioridade;
}fila_t;

fila_t* inicializa_fila(TIPO_FILA tipo);

p_elemento_t* inicializa_elemento(processo_t* processo);

p_elemento_t* pega_inicio(fila_t* fila);

void insere_elemento(fila_t* fila, p_elemento_t* e);
p_elemento_t* remove_elemento(fila_t* fila, unsigned int pid);

void deleta_elemento(p_elemento_t* e);
void deleta_fila(fila_t* fila);

const char* checa_tipo(fila_t* fila);
void print_fila(fila_t* fila);

#endif