#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

//Cria um elemento que aponta para o processo
p_elemento_t* inicializa_elemento(processo_t* processo){
    p_elemento_t* i = malloc(sizeof(p_elemento_t));
    i->processo = processo;
    i->proximo = NULL;
    return i;
}
/*Criacao da fila com alocação dinamica e um elemento fim para rápida insercao*/
fila_t* inicializa_fila(TIPO_FILA tipo){
    fila_t* fila = malloc(sizeof(fila_t));

    fila->inicio = NULL;
    fila->tipo = tipo;
    if(tipo == FILA_BAIXA_PRIORIDADE || tipo == FILA_DISCO)
        fila->prioridade = BAIXA;
    else
        fila->prioridade = ALTA;
    fila->tamanho = 0;

    return fila;
}

p_elemento_t* pega_inicio(fila_t* fila){
        return fila->inicio;
}

//Insere um elemento na fila (mas não cria)
void insere_elemento(fila_t* fila, p_elemento_t* i){
    if(fila->inicio == NULL){
        fila->inicio = i;
    }else{
        p_elemento_t* e = fila->inicio;
        while(e->proximo != NULL)
            e = e->proximo;
        e->proximo = i;
    }
    fila->tamanho++;
}

//Utilizado quando se quer remover um elemento da fila mas não deletá-lo
p_elemento_t* remove_elemento(fila_t* fila, unsigned int pid){
    if(fila->inicio == NULL)
        return NULL;

    p_elemento_t* e = NULL;
    if(fila->inicio->processo->pid == pid){
        e = fila->inicio;
        fila->inicio = fila->inicio->proximo;
    }else{  
        p_elemento_t* anterior = fila->inicio;
        e = fila->inicio->proximo;
        while(e->processo->pid != pid){
            anterior = e;
            e = anterior->proximo;
        }
        anterior->proximo = e->proximo;
        
    }
    e->proximo = NULL;
    fila->tamanho--;
    return e;
}

void deleta_elemento(p_elemento_t* e){
    deleta_processo(e->processo);
    free(e);
}

const char* checa_tipo(fila_t* fila){
    switch(fila->tipo){
        case FILA_ALTA_PRIORIDADE:
            return "FILA DE ALTA PRIORIDADE";
        case FILA_BAIXA_PRIORIDADE:
            return "FILA DE BAIXA PRIORIDADE";
        case FILA_DISCO:
            return "FILA DE DISCO";
        case FILA_FITA:
            return "FILA DE FITA MAGNETICA";
        case FILA_IMPRESSORA:
            return "FILA DE IMPRESSORA";
    }
    return "NONE";
}

void print_fila(fila_t* fila){
    printf("Tipo de Fila: %s\n", checa_tipo(fila));
    p_elemento_t* atual = fila->inicio;
    while(atual != NULL){
        print_processo(atual->processo);
        atual = atual->proximo;
    }
}