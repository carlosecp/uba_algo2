#ifndef AUXILIARES_H_
#define AUXILIARES_H_

#include "abb.h"

#define ERROR NULL
#define VACIO 0
#define NO_ENCONTRADO NULL

/*
 * Toma un ABB e inicia a recorrerlo de manera recursiva hasta
 * encontrar la ubicacion en la que insertar el nuevo elemento. Este recorrido
 * se hace utilizando la funcion de comparacion del ABB.
 *
 * En caso de exito retorna el ABB con el nuevo elemento insertado. En caso
 * contrario return NULL.
 */
nodo_abb_t* abb_insertar_recursivo_aux(nodo_abb_t* raiz, void* elemento, abb_comparador comparador, bool* insercion_exitosa);

/*
 * Toma un ABB e inicia a recorrerlo de manera recursiva hasta
 * encontrar el elemento a eliminar.
 *
 * En caso de encontrar el elemento y que se haga la eliminacion exitosa,
 * retorna el elemento almacenado previamente en el nodo eliminado. En caso
 * contrario retorna NULL.
 */
nodo_abb_t* abb_quitar_recursivo_aux(nodo_abb_t* raiz, void* elemento, void** elemento_quitado, abb_comparador comparador, bool* quitado_exitoso);

/*
 * Toma un ABB e inicia a recorrelo siempre hacia el nodo mas a la derecha,
 * buscando el nodo maximo del subarbol.
 *
 * En caso de exito retorna una referencia al nodo maximo encontrado. En caso
 * de error retorna NULL.
 */
nodo_abb_t* abb_extraer_maximo(nodo_abb_t** raiz);

/*
 * Toma un ABB e inicia a recorrelo recursivamente en busqueda de un
 * nodo que contenga el valor buscado, utilizando el una funcion de comparcion
 * dada.
 *
 * En caso de exito retorna el valor buscado, en caso contrario retorna NULL.
 */
void* abb_buscar_recursivo_aux(nodo_abb_t* raiz, void* elemento, abb_comparador comparador);

/*
 * Toma un ABB e inicia a recorrelo en un recorrido PREORDEN de manera recursiva,
 * al mismo tiempo en el que va agregando cada uno de los elementos recorridos
 * a un vector e incrementando el tamanio del mismo.
 *
 * Retorna la cantidad de elemento recorridos (0 en caso de error).
 */
size_t abb_recorrer_preorden_aux(nodo_abb_t* raiz, void** array, size_t tamanio_array);

/*
 * Toma un ABB e inicia a recorrelo en un recorrido INORDEN de manera recursiva,
 * al mismo tiempo en el que va agregando cada uno de los elementos recorridos
 * a un vector e incrementando el tamanio del mismo.
 *
 * Retorna la cantidad de elemento recorridos (0 en caso de error).
 */
size_t abb_recorrer_inorden_aux(nodo_abb_t* raiz, void** array, size_t tamanio_array);

/*
 * Toma un ABB e inicia a recorrelo en un recorrido POSTORDEN de manera recursiva,
 * al mismo tiempo en el que va agregando cada uno de los elementos recorridos
 * a un vector e incrementando el tamanio del mismo.
 *
 * Retorna la cantidad de elemento recorridos (0 en caso de error).
 */
size_t abb_recorrer_postorden_aux(nodo_abb_t* raiz, void** array, size_t tamanio_array);

/*
 * Toma un ABB e inicia a recorrelo en un recorrido PREORDEN de manera recursiva
 * al mismo tiempo en el que va evaluando cada uno de los elementos recorridos
 * con una funcion dada, que en caso de error retorna e incrementando el
 * tamanio del mismo.
 *
 * Retorna la cantidad de elemento recorridos (0 en caso de error).
 */
size_t abb_con_cada_elemento_preorden_aux(nodo_abb_t* raiz, bool (*funcion)(void*, void*), void* aux, bool* continuar_recorrido);

/*
 * Toma un ABB e inicia a recorrelo en un recorrido INORDEN de manera recursiva
 * al mismo tiempo en el que va evaluando cada uno de los elementos recorridos
 * con una funcion dada, que en caso de error retorna e incrementando el
 * tamanio del mismo.
 *
 * Retorna la cantidad de elemento recorridos (0 en caso de error).
 */
size_t abb_con_cada_elemento_inorden_aux(nodo_abb_t* raiz, bool (*funcion)(void*, void*), void* aux, bool* continuar_recorrido);

/*
 * Toma un ABB e inicia a recorrelo en un recorrido POSTORDEN de manera recursiva
 * al mismo tiempo en el que va evaluando cada uno de los elementos recorridos
 * con una funcion dada, que en caso de error retorna e incrementando el
 * tamanio del mismo.
 *
 * Retorna la cantidad de elemento recorridos (0 en caso de error).
 */
size_t abb_con_cada_elemento_postorden_aux(nodo_abb_t* raiz, bool (*funcion)(void*, void*), void* aux, bool* continuar_recorrido);

/*
 * Toma un ABB e inicia a recorrelo recursivamente y va destruyendo cada
 * uno de los nodos (liberandolos de la memoria). En caso de que se le pase
 * una funcion destructor, tambien destruye el elemento almacenado dentro del nodo.
 */
void abb_destruir_recursivo_aux(nodo_abb_t* raiz, void (*destructor)(void*));

#endif
