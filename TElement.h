//
// Created by sergio on 14/12/22.
//

#define PRACTICAMINISHELL_TELEMENTO_H

//#include "parser.h"
#include <unistd.h>


typedef struct{
    pid_t *pids;
    char *line;
    int ncommands;
} TElemento;

void crearElemento(pid_t *pidAlmacen, char *line, int ncommands, TElemento *e);

void mostrarElemento(TElemento *e);

void asignarElemento(TElemento *e1, TElemento *e2);

int igualElemento(TElemento *e1, TElemento *e2);


