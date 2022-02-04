#include "pila.h"

#include "lista.h"

pila_t*
pila_crear()
{
    return (pila_t*)calloc(1, sizeof(lista_t));
}

pila_t*
pila_apilar(pila_t* pila, void* elemento)
{
    return (pila_t*)lista_insertar((lista_t*)pila, elemento);
}

void*
pila_desapilar(pila_t* pila)
{
    return lista_quitar_de_posicion((lista_t*)pila, pila_tamanio(pila));
}

void*
pila_tope(pila_t* pila)
{
    return lista_ultimo((lista_t*)pila);
}

size_t
pila_tamanio(pila_t* pila)
{
    return lista_tamanio((lista_t*)pila);
}

bool
pila_vacia(pila_t* pila)
{
    return lista_vacia((lista_t*)pila);
}

void
pila_destruir(pila_t* pila)
{
    lista_destruir((lista_t*)pila);
}
