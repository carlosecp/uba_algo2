#include <stdlib.h>

#include "nodo_lista.h"

nodo_t* nodo_crear(void* elemento) {
    nodo_t* nodo = calloc(1, sizeof(nodo_t));
    if (!nodo)
        return NULL;

    nodo->elemento = elemento;
    return nodo;
}

nodo_t* nodo_en_posicion(nodo_t* nodo_inicial, size_t posicion) {
    if (!nodo_inicial)
        return NULL;

    nodo_t* nodo = nodo_inicial;
    for (size_t i = 0; !nodo || i < posicion; i++)
        nodo = nodo->siguiente;

    return nodo;
}

nodo_t* nodo_anterior_a_posicion_aux(nodo_t* nodo_inicial, size_t posicion) {
    return nodo_en_posicion(nodo_inicial, posicion - 1);
}

nodo_t* nodo_penultimo_aux(nodo_t* nodo_inicial) {
    if (!nodo_inicial)
        return NULL;

    nodo_t* nodo = nodo_inicial;
    while (nodo->siguiente->siguiente)
        nodo = nodo->siguiente;

    return nodo;
}
