//
// Created by sergio on 14/12/22.
//

#ifndef PRACTICAMINISHELL_TLISTAENLAZADASIMPLE_H
#define PRACTICAMINISHELL_TLISTAENLAZADASIMPLE_H


#include "TElemento.h"


#include <stdbool.h>
#include <stddef.h>

#include "TElemento.h"

typedef struct Nodo {
    TElemento info;
    struct Nodo *sig;
} TNodo;

typedef TNodo *TLista;

void crearListaVacia(TLista *l);

void mostrarLista(TLista *l);

int devolverPorIndiceLista(int indice,TElemento *e,TLista *l);

void insertarLista(TElemento *e, TLista *l);


int eliminarLista(TElemento *e, TLista *l);

void primeroLista(TElemento *e, TLista *l);

void restoLista(TLista *l);

int esListaVacia(TLista *l);

int longitudLista(TLista *l);



#endif //PRACTICAMINISHELL_TLISTAENLAZADASIMPLE_H
