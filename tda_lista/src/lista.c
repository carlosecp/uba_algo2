#include "lista.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "nodo.h"

lista_t*
lista_crear()
{
    return calloc(1, sizeof(lista_t));
}

lista_t*
lista_insertar(lista_t* lista, void* elemento)
{
    return lista_insertar_en_posicion(lista, elemento, lista_tamanio(lista));
}

lista_t*
lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion)
{
    if (!lista)
        return NULL;

    nodo_t* nodo = nodo_crear(elemento);
    if (!nodo)
        return NULL;

    if (!(lista -> nodo_inicio))
        lista -> nodo_inicio = lista -> nodo_fin = nodo;

    else if (posicion == 0) {
        nodo -> siguiente = lista -> nodo_inicio;
        lista -> nodo_inicio = nodo;
    }

    else if (posicion >= lista_tamanio(lista)) {
        lista -> nodo_fin -> siguiente = nodo;
        lista -> nodo_fin = nodo;
    }

    else {
        nodo_t* nodo_anterior_al_insertado =
            nodo_anterior_a_posicion_aux(lista -> nodo_inicio, posicion);
        if (!nodo_anterior_al_insertado)
            return NULL;

        nodo -> siguiente = nodo_anterior_al_insertado -> siguiente;
        nodo_anterior_al_insertado -> siguiente = nodo;
    }

    lista -> cantidad++;
    return lista;
}

void*
lista_quitar(lista_t* lista)
{
    return lista_quitar_de_posicion(lista, lista_tamanio(lista));
}

void*
lista_quitar_de_posicion(lista_t* lista, size_t posicion)
{
    size_t tamanio = 0;
    void* elemento_quitado = NULL;

    if (!lista || !(tamanio = lista_tamanio(lista)))
        return NULL;

    if (tamanio == 1) {
        elemento_quitado = lista_primero(lista);
        free(lista -> nodo_inicio);

        lista -> nodo_inicio = lista -> nodo_fin = NULL;
    }

    else if (posicion == 0) {
        nodo_t* nodo_incial_actual = lista -> nodo_inicio;
        lista -> nodo_inicio = lista -> nodo_inicio -> siguiente;

        elemento_quitado = nodo_incial_actual -> elemento;
        free(nodo_incial_actual);
    }

    else if (posicion >= tamanio) {
        nodo_t* nodo_penultimo = nodo_penultimo_aux(lista -> nodo_inicio);
        if (!nodo_penultimo)
            return NULL;

        elemento_quitado = nodo_penultimo -> siguiente -> elemento;
        free(nodo_penultimo -> siguiente);

        nodo_penultimo -> siguiente = NULL;
        lista -> nodo_fin = nodo_penultimo;
    }

    else {
        nodo_t* nodo_anterior_al_quitado =
            nodo_anterior_a_posicion_aux(lista -> nodo_inicio, posicion);
        if (!nodo_anterior_al_quitado)
            return NULL;

        nodo_t* nodo_quitado = nodo_anterior_al_quitado -> siguiente;
        elemento_quitado = nodo_quitado -> elemento;

        nodo_anterior_al_quitado -> siguiente = nodo_quitado -> siguiente;
        free(nodo_quitado);
    }

    lista -> cantidad--;
    return elemento_quitado;
}

void*
lista_elemento_en_posicion(lista_t* lista, size_t posicion)
{
    size_t tamanio = 0;
    if (!lista || posicion > (tamanio = lista_tamanio(lista)))
        return NULL;

    if (posicion == 0)
        return lista_primero(lista);

    if (posicion >= tamanio)
        return lista_ultimo(lista);

    nodo_t* nodo = nodo_en_posicion(lista->nodo_inicio, posicion);
    if (!nodo)
        return NULL;

    return nodo->elemento;
}

void*
lista_primero(lista_t* lista)
{
    if (!lista || !(lista->nodo_inicio))
        return NULL;

    return lista->nodo_inicio->elemento;
}

void*
lista_ultimo(lista_t* lista)
{
    if (!lista || !(lista->nodo_fin))
        return NULL;

    return lista->nodo_fin->elemento;
}

bool
lista_vacia(lista_t* lista)
{
    if (!lista)
        return true;

    return lista_tamanio(lista) == 0;
}

size_t
lista_tamanio(lista_t* lista)
{
    if (!lista)
        return 0;

    return lista -> cantidad;
}

void
lista_destruir(lista_t* lista)
{
    if (!lista)
        return;

    while (lista -> nodo_inicio) {
        nodo_t* nodo_actual = lista -> nodo_inicio;
        lista -> nodo_inicio = nodo_actual -> siguiente;
        free(nodo_actual);
    }

    free(lista);
}

lista_iterador_t*
lista_iterador_crear(lista_t* lista)
{
    if (!lista)
        return NULL;

    lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));
    if (!iterador)
        return NULL;

    iterador -> lista = lista;
    iterador -> corriente = lista -> nodo_inicio;

    return iterador;
}

bool
lista_iterador_tiene_siguiente(lista_iterador_t* iterador)
{
    if (!iterador)
        return false;

    return iterador -> corriente != NULL;
}

bool
lista_iterador_avanzar(lista_iterador_t* iterador)
{
    if (!iterador || !iterador -> corriente)
        return false;

    iterador -> corriente = iterador -> corriente -> siguiente;
    return iterador -> corriente != NULL;
}

void*
lista_iterador_elemento_actual(lista_iterador_t* iterador)
{
    if (!iterador || !iterador -> corriente)
        return NULL;

    return iterador -> corriente -> elemento;
}

void
lista_iterador_destruir(lista_iterador_t* iterador)
{
    if (!iterador)
        return;

    free(iterador);
}

size_t
lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void* contexto)
{
    if (!lista || !funcion)
        return 0;

    size_t elementos_iterados = 0;
    nodo_t* nodo_actual = lista -> nodo_inicio;

    while (nodo_actual && funcion(nodo_actual -> elemento, contexto)) {
        nodo_actual = nodo_actual -> siguiente;
        elementos_iterados++;
    }

    return elementos_iterados;
}