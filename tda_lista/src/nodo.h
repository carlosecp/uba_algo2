#ifndef NODO_H_
#define NODO_H_

#include "lista.h"

/**
 * Crea un nodo enlazada para almacenar el elemento deseado.
 * Inicialmente el nodo siguiente al nodo creado es NULL.
 * 
 * En caso de exito retorna el nodo creado, en caso constrario
 * devuelve NULL.
 */
nodo_t* nodo_crear(void* elemento);

/**
 * Busca un nodo en una posicion especifica.
 * 
 * En caso de exito retorna el nodo en la posicion deseada,
 * en caso contrario o si no se encuentra el nodo retorna NULL.
 */
nodo_t* nodo_en_posicion(nodo_t* inicio, size_t posicion);

/**
 * Busca el nodo anterior a la posicion deseada.
 * Funciona a manera de auxiliar de la funcion nodo_en_posicion.
 */
nodo_t* nodo_anterior_a_posicion_aux(nodo_t* nodo_inicial, size_t posicion);

/**
 * Busca el penultimo nodo de la lista.
 * 
 * En caso de exito retorna el nodo encontrado, o NULL en caso de error.
 */
nodo_t* nodo_penultimo_aux(nodo_t* nodo_inicial);

#endif