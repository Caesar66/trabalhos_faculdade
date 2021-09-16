#ifndef VALORES_H
#define VALORES_H

#define QUANTUM 2

#define LIMITE_RESERVA 5
#define LIMITE_PROCESSOS_SIMULTANEOS 10
#define LIMITE_PROCESSOS_MAX 20
#define LIMITE_CICLOS 400

#define MAX_TEMPO_ESPERA 7 //[10] Tempo de Espera em Fila de Baixa Prioridade

#define MAX_TEMPO_ATIVACAO 5 //[5] Maior Tempo Entre Criacao e Ativacao do Programa
#define MAX_TEMPO_EXECUCAO 10 //[16] Maior Tempo de Execucao

#define MAX_TEMPO_IO_ATIVACAO 4 //[3] Maior Tempo Para Comeco da Execucao de IO
#define MAX_TEMPO_IO_EXECUCAO 5 //[9] Maior Tempo de Execucao de IO

typedef enum{
    NOVO,
    PRONTO,
    EXECUCAO,
    BLOQUEADO,
    SAIDA
}STATUS;

typedef enum{
    BAIXA,
    ALTA
}PRIORIDADE;

typedef enum{
    FILA_ALTA_PRIORIDADE,
    FILA_BAIXA_PRIORIDADE,
    FILA_DISCO,
    FILA_IMPRESSORA,
    FILA_FITA
}TIPO_FILA;

typedef enum{
    DISCO,
    IMPRESSORA,
    FITA_MAGNETICA
}IO_OPERACAO;

#endif