#include <stdlib.h>
#include <stdio.h>

#include "controlador_processo.h"
#include "criador_processo.h"
#include "fila.h"
#include "valores.h"
#include "processo.h"

controlador_t* inicializa_control(void){
    controlador_t* control = malloc(sizeof(controlador_t));

    control->contadorTempo = 0;
    control->contadorQuantum = 0;

    control->filaAltaPrioridade = inicializa_fila(FILA_ALTA_PRIORIDADE);
    control->filaBaixaPrioridade = inicializa_fila(FILA_BAIXA_PRIORIDADE);

    control->filaDisco = inicializa_fila(FILA_DISCO);
    control->filaFita = inicializa_fila(FILA_FITA);
    control->filaImpressora = inicializa_fila(FILA_IMPRESSORA);

    control->processoEmExecucao = NULL;

    control->IODisco = NULL;
    control->IOFita = NULL;
    control->IOImpressora = NULL;

    return control;
}

void deleta_control(controlador_t* ct){
    free(ct);
}

void muda_fila(fila_t* fila_antiga, fila_t* fila_nova, unsigned int pid){
    p_elemento_t* e = remove_elemento(fila_antiga, pid);
    insere_elemento(fila_nova, e);
}

void redireciona_processo(controlador_t* ct, fila_t* fila){
    TIPO_FILA tipo = fila->tipo;
    p_elemento_t* e = remove_elemento(fila, pega_inicio(fila)->processo->pid);
    switch(tipo){
        case FILA_ALTA_PRIORIDADE:
            ct->processoEmExecucao = e;
            muda_estado(e->processo, EXECUCAO);
            break;
        case FILA_DISCO:
            ct->IODisco = e;
            break;
        case FILA_FITA:
            ct->IOFita = e;
            break;
        case FILA_IMPRESSORA:
            ct->IOImpressora = e;
            break;
        default:
            break;
    }
}

p_elemento_t* pega_processo_execucao(controlador_t* ct){
    return ct->processoEmExecucao;
}

void coloca_elemento_fila_io(controlador_t* ct, p_elemento_t* e){
    if(e == NULL)
        return;
    switch(pega_io(e->processo)->tipo){
        case DISCO:
            insere_elemento(ct->filaDisco, e);
            break;
        case FITA_MAGNETICA:
            insere_elemento(ct->filaFita, e);
            break;
        case IMPRESSORA:
            insere_elemento(ct->filaImpressora, e);
            break;
    }
}

int checa_filas_vazias(controlador_t* ct){
    int vaziaFilaAlta = ct->filaAltaPrioridade->inicio == NULL;
    int vaziaFilaBaixa = ct->filaBaixaPrioridade->inicio == NULL;
    int vaziaFilaDisco = ct->filaDisco->inicio == NULL;
    int vaziaFilaImpressora = ct->filaImpressora->inicio == NULL;
    int vaziaFilaFita = ct->filaFita->inicio == NULL;
    return vaziaFilaAlta && vaziaFilaBaixa && vaziaFilaDisco && vaziaFilaImpressora && vaziaFilaFita;
}

void executa_processo_emExecucao(controlador_t* control, cr_processo_t* criador){
    printf("\n\nCICLO %d\n", control->contadorTempo);
    //Checa Processo em Execucao
    if(pega_processo_execucao(control) != NULL){
        //Se finalizou, printa e sai
        if(checa_fim_execucao(control->processoEmExecucao->processo)){
            muda_estado(pega_processo_execucao(control)->processo, SAIDA);
            deleta_elemento(control->processoEmExecucao);
            decrementa_quantidadeProcessosAtuais(criador);
            control->processoEmExecucao = NULL;

        //Se chegou tempo de IO, coloca bloqueado, printa, tira e bota na fila do IO
        }else{
            if((pega_io(pega_processo_execucao(control)->processo) != NULL) && pega_processo_execucao(control)->processo->tempoEmExecucao == pega_io(pega_processo_execucao(control)->processo)->tempoChegada){
                    muda_estado(pega_processo_execucao(control)->processo, BLOQUEADO);
                    coloca_elemento_fila_io(control, control->processoEmExecucao);
                    control->processoEmExecucao = NULL;

            //Se terminou quantum, printa, zera contadorQuantum, bota em PRONTO 
            // coloca na fila de baixa prioridade
            }else{
                if(control->contadorQuantum == QUANTUM){
                    //Modifica status e coloca na fila de baixa prioridade
                    muda_prioridade(pega_processo_execucao(control)->processo, BAIXA);
                    muda_estado(pega_processo_execucao(control)->processo, PRONTO);
                    insere_elemento(control->filaBaixaPrioridade, control->processoEmExecucao);
                    //Tira o processo da Execucao e reseta contagem do Quantum
                    control->processoEmExecucao = NULL;
                    control->contadorQuantum = 0;

                //Se nenhum dos anteriores, mantem
                }else{
                    //Sobe uma faixa de tempo para processo em execucao e o contador de quantum
                    incrementa_tempoEmExecucao(control->processoEmExecucao->processo);
                    incrementa_tempoDeVida(control->processoEmExecucao->processo);
                    control->contadorQuantum++;
                    printf("PROCESSO EM EXECUCAO:\n");
                    print_processo(control->processoEmExecucao->processo);
                }
            }
        }
    }
}

void checa_fila_altaPrioridade(controlador_t* control){
    //Checa Fila de Alta Prioridade
    //Se nao tem processo em Execucao, coloca em Execucao o primeiro da fila
    if(pega_processo_execucao(control) == NULL && pega_inicio(control->filaAltaPrioridade) != NULL){
        incrementa_tempoDeVida(pega_inicio(control->filaAltaPrioridade)->processo);
        redireciona_processo(control, control->filaAltaPrioridade);
        printf("PROCESSO EM EXECUCAO:\n");
        print_processo(control->processoEmExecucao->processo);
    }
    //Incrementa tempo de Espera em Fila de Alta Prioridade
    p_elemento_t* e = pega_inicio(control->filaAltaPrioridade);
    printf("\nFILA DE ALTA PRIORIDADE:\n");
    while(e != NULL){
        incrementa_tempoDeVida(e->processo);
        print_processo(e->processo);
        e = e->proximo;
    }
}
void checa_fila_baixaPrioridade(controlador_t* control){
    //Checa Fila de Baixa Prioridade
    //Incrementa tempo de Espera em Fila de Baixa Prioridade
    //  e checa se Processo bateu limite do tempo de Espera 
    //   - Se sim, transfere para fila de alta prioridade, coloca tempoEmEspera em 0
    //   - Se nao, mantem 
    p_elemento_t* e = pega_inicio(control->filaBaixaPrioridade);
    printf("\nFILA DE BAIXA PRIORIDADE:\n");
    while(e != NULL){
        incrementa_tempoEmEspera(e->processo);
        incrementa_tempoDeVida(e->processo);
        print_processo(e->processo);
        if(!checa_lim_tempoEmEspera(e->processo)){
            muda_prioridade(e->processo, ALTA);
            muda_fila(control->filaBaixaPrioridade, control->filaAltaPrioridade, e->processo->pid);
            e->processo->tempoEmEspera = 0;
        }
        e = e->proximo;
    }
}

void checa_fila_io(controlador_t* control, IO_OPERACAO io_tipo){
    //Checa IO Disco
    p_elemento_t* io_centro = NULL;
    fila_t* fila = NULL;
    switch(io_tipo){
        case DISCO:
            io_centro = control->IODisco;
            fila = control->filaDisco;
            break;
        case FITA_MAGNETICA:
            io_centro = control->IOFita;
            fila = control->filaFita;
            break;
        case IMPRESSORA:
            io_centro = control->IOImpressora;
            fila = control->filaImpressora;
            break;
        default:
            return;
    }
    if(io_centro != NULL){
        printf("\nI/O CENTRO OCUPADO");
        //Checa se acabou o IO
        if(pega_io(io_centro->processo)->quantum_io == pega_io(io_centro->processo)->tempoFinalizacao){
            muda_estado(io_centro->processo, PRONTO);
            if(pega_tipo_io(pega_io(io_centro->processo)) == DISCO)
                insere_elemento(control->filaBaixaPrioridade, io_centro);
            else
                insere_elemento(control->filaAltaPrioridade, io_centro);
            switch(io_tipo){
                case DISCO:
                    control->IODisco = NULL;
                    break;
                case FITA_MAGNETICA:
                    control->IOFita = NULL;
                    break;
                case IMPRESSORA:
                    control->IOImpressora = NULL;
                    break;
                default:
                    break;
            }

        //Se nao acabou, mantem
        }else{
            incrementa_quantum_io(io_centro->processo->io);
            incrementa_tempoDeVida(io_centro->processo);
            printf("\nPROCESSO NO %s:\n", pega_tipo_io(io_centro->processo->io));
            print_processo(io_centro->processo);
        }
    }
    p_elemento_t* e = pega_inicio(fila);
    if(e != NULL){
        printf("\nFILA DE %s:\n", pega_tipo_io(e->processo->io));
        while(e != NULL){
            incrementa_tempoDeVida(e->processo);
            print_processo(e->processo);
            e = e->proximo;
        }
        if(io_centro == NULL){
            switch(io_tipo){
                case DISCO:
                    control->IODisco = remove_elemento(fila, pega_inicio(fila)->processo->pid);
                    break;
                case FITA_MAGNETICA:
                    control->IOFita = remove_elemento(fila, pega_inicio(fila)->processo->pid);
                    break;
                case IMPRESSORA:
                    control->IOImpressora = remove_elemento(fila, pega_inicio(fila)->processo->pid);
                    break;
                default:
                    break;
            }
            //redireciona_proces    so(control, fila);
        }
    }
}

void checa_reserva(controlador_t* control, cr_processo_t* criador){
    //Checa Reserva
    printf("\nRESERVA:\n");
    if(criador->ocupacaoReserva > 0){
        for(int i = 0; i < criador->ocupacaoReserva; i++){
            incrementa_tempoDeVida(criador->reserva[i]);
            print_processo(criador->reserva[i]);
            if(criador->reserva[i]->tempoDeVida == criador->reserva[i]->tempoChegada){
                p_elemento_t* e = inicializa_elemento(criador->reserva[i]);
                muda_estado(e->processo, BAIXA);
                insere_elemento(control->filaAltaPrioridade, e);
                criador->ocupacaoReserva--;
                for(int j = i; j < criador->ocupacaoReserva; j++)
                    criador->reserva[j] = criador->reserva[j+1];
            }
        }
    }
}

void checa_criacao_processos(controlador_t* control, cr_processo_t* criador){
    if(checa_limite_max(criador)){
        //Checa limite de processos ativos
        if(checa_limite_atual(criador)){
            //Checa se reserva disponível
            if(checa_limite_reserva(criador)){
                //Cria Processo e coloca na reserva
                processo_t* p = cria_processo(criador->pidControl);
                incrementa_controle_pid(criador);
                incrementa_quantidadeProcessosAtuais(criador);
                incrementa_quantidadeProcessosCriados(criador);
                //Checa se tem tempo de Chegada, se não, coloca na fila de Alta Prioridade
                if(p->tempoChegada > 0){
                    criador->reserva[criador->ocupacaoReserva] = p;
                    criador->ocupacaoReserva++;
                }else{
                    p_elemento_t* e = inicializa_elemento(p);
                    e->processo->estado = PRONTO;
                    insere_elemento(control->filaAltaPrioridade, e);
                }
            }
        }
    }
}

