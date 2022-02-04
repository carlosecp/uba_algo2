#include "auxiliares_abb.h"

#include <stdio.h>

nodo_abb_t* abb_insertar_recursivo_aux(nodo_abb_t* raiz, void* elemento, abb_comparador comparador, bool* insercion_exitosa) {
    if (!comparador)
        return ERROR;

    if (!raiz) {
        nodo_abb_t* nuevo_nodo = calloc(1, sizeof(nodo_abb_t));
        if (!nuevo_nodo)
            return ERROR;

        nuevo_nodo->elemento = elemento;
        *insercion_exitosa = true;

        return nuevo_nodo;
    }

    int comparacion = comparador(elemento, raiz->elemento);

    if (comparacion <= 0)
        raiz->izquierda = abb_insertar_recursivo_aux(raiz->izquierda, elemento, comparador, insercion_exitosa);
    else
        raiz->derecha = abb_insertar_recursivo_aux(raiz->derecha, elemento, comparador, insercion_exitosa);

    return raiz;
}

nodo_abb_t* abb_quitar_recursivo_aux(nodo_abb_t* raiz, void* elemento, void** elemento_quitado, abb_comparador comparador, bool* quitado_exitoso) {
    if (!raiz || !comparador)
        return NO_ENCONTRADO;

    int comparacion = comparador(elemento, raiz->elemento);
    if (comparacion == 0) {
        if (raiz->izquierda && raiz->derecha) {
            nodo_abb_t* predecesor_inmediato = abb_extraer_maximo(&(raiz->izquierda));

            predecesor_inmediato->izquierda = raiz->izquierda;
            predecesor_inmediato->derecha = raiz->derecha;

            *elemento_quitado = raiz->elemento;
            free(raiz);

            *quitado_exitoso = true;
            return predecesor_inmediato;
        } else {
            nodo_abb_t* hijo = raiz->derecha ? raiz->derecha : raiz->izquierda;
            *elemento_quitado = raiz->elemento;
            free(raiz);

            *quitado_exitoso = true;
            return hijo;
        }
    } else if (comparacion < 0) {
        raiz->izquierda = abb_quitar_recursivo_aux(raiz->izquierda, elemento, elemento_quitado, comparador, quitado_exitoso);
    } else {
        raiz->derecha = abb_quitar_recursivo_aux(raiz->derecha, elemento, elemento_quitado, comparador, quitado_exitoso);
    }

    return raiz;
}

nodo_abb_t* abb_extraer_maximo(nodo_abb_t** raiz) {
    if (!(*raiz))
        return NULL;

    if (!((*raiz)->derecha)) {
        nodo_abb_t* nodo_maximo = *raiz;
        *raiz = (*raiz)->izquierda;
        return nodo_maximo;
    }

    return abb_extraer_maximo(&((*raiz)->derecha));
}

void* abb_buscar_recursivo_aux(nodo_abb_t* raiz, void* elemento, abb_comparador comparador) {
    if (!raiz || !comparador)
        return NO_ENCONTRADO;

    int comparacion = comparador(elemento, raiz->elemento);
    if (comparacion == 0)
        return raiz->elemento;
    else if (comparacion < 0)
        return abb_buscar_recursivo_aux(raiz->izquierda, elemento, comparador);
    else
        return abb_buscar_recursivo_aux(raiz->derecha, elemento, comparador);
}

size_t abb_recorrer_preorden_aux(nodo_abb_t* raiz, void** array, size_t tamanio_array) {
    if (!raiz || !tamanio_array)
        return 0;

    size_t cantidad = 0;
    array[cantidad++] = raiz->elemento;
    cantidad += abb_recorrer_preorden_aux(raiz->izquierda, array + cantidad, tamanio_array - cantidad);
    cantidad += abb_recorrer_preorden_aux(raiz->derecha, array + cantidad, tamanio_array - cantidad);

    return cantidad;
}

size_t abb_recorrer_inorden_aux(nodo_abb_t* raiz, void** array, size_t tamanio_array) {
    if (!raiz || !tamanio_array)
        return 0;

    size_t cantidad = 0;
    cantidad += abb_recorrer_inorden_aux(raiz->izquierda, array + cantidad, tamanio_array - cantidad);
    array[cantidad++] = raiz->elemento;
    cantidad += abb_recorrer_inorden_aux(raiz->derecha, array + cantidad, tamanio_array - cantidad);

    return cantidad;
}

size_t abb_recorrer_postorden_aux(nodo_abb_t* raiz, void** array, size_t tamanio_array) {
    if (!raiz || !tamanio_array)
        return 0;

    size_t cantidad = 0;
    cantidad += abb_recorrer_postorden_aux(raiz->izquierda, array + cantidad, tamanio_array - cantidad);
    cantidad += abb_recorrer_postorden_aux(raiz->derecha, array + cantidad, tamanio_array - cantidad);

    if (tamanio_array - cantidad)
        array[cantidad++] = raiz->elemento;

    return cantidad;
}

size_t abb_con_cada_elemento_preorden_aux(nodo_abb_t* raiz, bool (*funcion)(void*, void*), void* aux, bool* continuar_recorrido) {
    if (!raiz || !funcion || !continuar_recorrido || !(*continuar_recorrido))
        return 0;

    size_t cantidad = 1;

    if (!(*continuar_recorrido = funcion(raiz->elemento, aux)))
        return cantidad;

    cantidad += abb_con_cada_elemento_preorden_aux(raiz->izquierda, funcion, aux, continuar_recorrido);
    cantidad += abb_con_cada_elemento_preorden_aux(raiz->derecha, funcion, aux, continuar_recorrido);

    return cantidad;
}

size_t abb_con_cada_elemento_inorden_aux(nodo_abb_t* raiz, bool (*funcion)(void*, void*), void* aux, bool* continuar_recorrido) {
    if (!raiz || !funcion || !continuar_recorrido || !(*continuar_recorrido))
        return 0;

    size_t cantidad = 0;
    cantidad += abb_con_cada_elemento_inorden_aux(raiz->izquierda, funcion, aux, continuar_recorrido);

    if (!(*continuar_recorrido))
        return cantidad;

    *continuar_recorrido = funcion(raiz->elemento, aux);
    cantidad += 1;

    if (*continuar_recorrido)
        cantidad += abb_con_cada_elemento_inorden_aux(raiz->derecha, funcion, aux, continuar_recorrido);

    return cantidad;
}

size_t abb_con_cada_elemento_postorden_aux(nodo_abb_t* raiz, bool (*funcion)(void*, void*), void* aux, bool* continuar_recorrido) {
    if (!raiz || !funcion || !continuar_recorrido || !(*continuar_recorrido))
        return 0;

    size_t cantidad = 0;
    cantidad += abb_con_cada_elemento_postorden_aux(raiz->izquierda, funcion, aux, continuar_recorrido);
    cantidad += abb_con_cada_elemento_postorden_aux(raiz->derecha, funcion, aux, continuar_recorrido);

    if (!(*continuar_recorrido))
        return cantidad;

    *continuar_recorrido = funcion(raiz->elemento, aux);
    cantidad += 1;

    return cantidad;
}

void abb_destruir_recursivo_aux(nodo_abb_t* raiz, void (*destructor)(void*)) {
    if (!raiz)
        return;

    abb_destruir_recursivo_aux(raiz->izquierda, destructor);
    abb_destruir_recursivo_aux(raiz->derecha, destructor);

    if (destructor)
        destructor(raiz->elemento);

    free(raiz);
}
