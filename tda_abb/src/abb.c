#include "abb.h"

#include <stddef.h>
#include <stdlib.h>

#include <stdbool.h>
#include "auxiliares.h"

abb_t*
abb_crear(abb_comparador comparador)
{
    if (!comparador)
        return ERROR;

    abb_t* arbol = calloc(1, sizeof(abb_t));
    if (!arbol)
        return ERROR;

    arbol -> comparador = comparador;
    return arbol;
}

abb_t*
abb_insertar(abb_t* arbol, void* elemento)
{
    if (!arbol)
        return ERROR;

    bool insercion_exitosa = false;
    arbol -> nodo_raiz = abb_insertar_recursivo_aux(arbol -> nodo_raiz, elemento, arbol -> comparador, &insercion_exitosa);

	if (insercion_exitosa)
		arbol -> tamanio++;

    return arbol;
}

void*
abb_quitar(abb_t* arbol, void* elemento)
{
    if (!arbol)
        return ERROR;

    void* elemento_quitado = NULL;

	bool quitado_exitoso = false;
    arbol -> nodo_raiz = abb_quitar_recursivo_aux(arbol -> nodo_raiz, elemento, &elemento_quitado, arbol -> comparador, &quitado_exitoso);

	if (quitado_exitoso)
		arbol -> tamanio--;

    return elemento_quitado;
}

void*
abb_buscar(abb_t* arbol, void* elemento)
{
    if (!arbol)
        return ERROR;

    return abb_buscar_recursivo_aux(arbol -> nodo_raiz, elemento, arbol -> comparador);
}

bool
abb_vacio(abb_t* arbol)
{
    if (!arbol)
        return true;

    return arbol -> tamanio == 0;
}

size_t
abb_tamanio(abb_t* arbol)
{
    if (!arbol)
        return VACIO;

    return arbol -> tamanio;
}

void
abb_destruir(abb_t* arbol)
{
    abb_destruir_todo(arbol, NULL);
}

void
abb_destruir_todo(abb_t* arbol, void (*destructor)(void*))
{
    if (!arbol)
        return;

    abb_destruir_recursivo_aux(arbol -> nodo_raiz, destructor);
    free(arbol);
}

size_t
abb_con_cada_elemento(abb_t* arbol, abb_recorrido recorrido, bool (*funcion)(void*, void*), void* aux)
{
	if (!arbol || !funcion)
		return 0;

	bool continuar_recorrido = true;

	if (recorrido == PREORDEN)
		return abb_con_cada_elemento_preorden_aux(arbol -> nodo_raiz, funcion, aux, &continuar_recorrido);

	if (recorrido == INORDEN)
		return abb_con_cada_elemento_inorden_aux(arbol -> nodo_raiz, funcion, aux, &continuar_recorrido);

	if (recorrido == POSTORDEN)
		return abb_con_cada_elemento_postorden_aux(arbol -> nodo_raiz, funcion, aux, &continuar_recorrido);

	return 0;
}

size_t
abb_recorrer(abb_t* arbol, abb_recorrido recorrido, void** array, size_t tamanio_array)
{
    if (!arbol || !array)
        return 0;

    if (recorrido == PREORDEN)
        return abb_recorrer_preorden_aux(arbol -> nodo_raiz, array, tamanio_array);

    if (recorrido == INORDEN)
        return abb_recorrer_inorden_aux(arbol -> nodo_raiz, array, tamanio_array);

    if (recorrido == POSTORDEN)
        return abb_recorrer_postorden_aux(arbol -> nodo_raiz, array, tamanio_array);

    return 0;
}
