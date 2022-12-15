//
// Created by sergio on 14/12/22.
//

#include "TElement.h"
#include "string.h"
#include "stdio.h"

void crearElemento(pid_t *pids, char *line, int ncommands, TElemento *e) {
    e->pids = pids;
    e->line = strdup(line);
    e->ncommands = ncommands;
}

void mostrarElemento(TElemento *e) {
    printf("%s", e->line);
}

void asignarElemento(TElemento *e1, TElemento *e2) {
    e2->pids = e1->pids;
    e2->line = e1->line;
    e2->ncommands = e1->ncommands;
}

int igualElemento(TElemento *e1, TElemento *e2) {
    return e1->pids == e2->pids;
}


