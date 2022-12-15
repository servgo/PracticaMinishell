//
// Created by sergio on 14/12/22.
//

#include "TElement.h"
#include "string.h"
#include "stdio.h"

void crearElemento(pid_t *pidAlmacen, char *line, int ncommands, TElemento *e) {
    e->pidAlmacen = pidAlmacen;
    e->line = strdup(line);
    e->ncommands = ncommands;
}

void mostrarElemento(TElemento *e) {
    printf("%s", e->line);
}

void asignarElemento(TElemento *e1, TElemento *e2) {
    e2->pidAlmacen = e1->pidAlmacen;
    e2->line = e1->line;
    e2->ncommands = e1->ncommands;
}

int igualElemento(TElemento *e1, TElemento *e2) {
    return e1->pidAlmacen == e2->pidAlmacen;
}


