//
// Created by sergio on 14/12/22.
//


#include "parser.h"
#include <unistd.h>

typedef struct TBackground {
    pid_t *pidAlmacen;
    char *line;
    int ncommands;

} TElemento;

void crearElemento(pid_t *pidAlmacen, char *line, int ncommands, TElemento *e);

void mostrarElemento(TElemento *e);

void asignarElemento(TElemento *e1, TElemento *e2);

int igualElemento(TElemento *e1, TElemento *e2);

#ifndef PRACTICAMINISHELL_TELEMENTO_H
#define PRACTICAMINISHELL_TELEMENTO_H

#endif //PRACTICAMINISHELL_TELEMENTO_H
