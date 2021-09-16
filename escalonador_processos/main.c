#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "valores.h"
#include "fila.h"
#include "io.h"
#include "controlador_processo.h"
#include "processo.h"
#include "criador_processo.h"


int main(){
    srand(time(NULL));
    cr_processo_t* criador = inicializa_cr_processo();
    controlador_t* control = inicializa_control();

    int loop = 1;
    while(loop){
        executa_processo_emExecucao(control, criador);

        checa_fila_altaPrioridade(control);
        
        checa_fila_baixaPrioridade(control);
        
        checa_fila_io(control, DISCO);
        checa_fila_io(control, FITA_MAGNETICA);
        checa_fila_io(control, IMPRESSORA);
   
        checa_reserva(control, criador);
    
        checa_criacao_processos(control, criador);

        if((checa_filas_vazias(control) && (!checa_limite_max(criador)) || control->contadorTempo >= LIMITE_CICLOS))
            loop = 0;

        control->contadorTempo++;
        sleep(1);
    }
    return 0;
}