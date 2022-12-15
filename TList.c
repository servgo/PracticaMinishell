#include "TLista.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

void crearListaVacia(TLista *l) {
    *l = NULL;
}

void mostrarLista(TLista *l) {
    TLista aux = *l;
    int i = 1;
    while (aux != NULL) { //  while(aux)
        printf("[%d] Ejecutando    ", i);
        mostrarElemento(&aux->info);
        i++;
        aux = aux->sig;
    }
}

void insertarLista(TElemento *e, TLista *l) {
    TNodo *nuevo = malloc(sizeof(TNodo));
    asignarElemento(e, &nuevo->info);
    nuevo->sig = NULL;
    if (esListaVacia(l)) {
        *l = nuevo;
    } else {
        TNodo *ultimo = *l;
        while (ultimo->sig != NULL) {
            ultimo = ultimo->sig;
        }
        ultimo->sig = nuevo;
    }
}

int devolverPorIndiceLista(int indice,TElemento *e,TLista *l){
    if (longitudLista(l) < indice){
        printf("El pid introducido no existe");
        return -1;
    } else{
        TNodo * aux = *l;
        for(int i=1; i<indice; i++){
            aux = aux->sig;
        }
        asignarElemento(&aux->info,e);
        return 0;
    }
}

int eliminarLista(TElemento *e, TLista *l) { //borra la primera aparicion de e
    TNodo *act = *l;
    TNodo *ant = NULL;
    int encontrado = 0;
    while (act != NULL && (!encontrado)) {
        encontrado = igualElemento(&act->info, e);
        if (encontrado) {
            if (ant == NULL) { //primero de la lista
                (*l) = (*l)->sig;
            } else {
                ant->sig = act->sig;
            }
            free(act);
        } else { //no es el actual
            ant = act;
            act = act->sig;
        }
    }
    return encontrado;
}

void primeroLista(TElemento *e, TLista *l) { //Condicion: Lista No vacia
    if (!esListaVacia(l)) {
        asignarElemento(&(*l)->info, e);
    } else {
        printf("No existen procesos en segundo plano\n");
    }
}

void restoLista(TLista *l) { //Condicion: Lista No vacia
    if (!esListaVacia(l)) {
        TLista aux = *l;
        *l = (*l)->sig;
        free(aux);
    }
}

int esListaVacia(TLista *l) {
    return *l == NULL;//return !(*l)
}

int longitudLista(TLista *l) {
    TLista aux = *l;
    int longitud = 0;
    while ((aux != NULL)) {
        longitud++;
        aux = aux->sig;
    }
    return longitud;
}
