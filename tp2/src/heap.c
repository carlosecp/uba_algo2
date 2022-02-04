#include "heap.h"

#include <stdlib.h>

#define MAX_INICIAL 10

/*
 * Crea un heap binario utilizando un vector generico y dinamico con un
 * tamanio inicial.
 *
 * En caso de exito retorna un puntero al heap creado y NULL en caso de error.
 */
heap_t* heap_crear(heap_comparador comparador) {
    if (!comparador)
        return NULL;

    heap_t* heap = malloc(sizeof(heap_t));

    heap->tamanio = 0;
    heap->maximo = MAX_INICIAL;
    heap->comparador = comparador;

    heap->elementos = calloc(heap->maximo, sizeof(void*));
    if (!(heap->elementos)) {
        free(heap);
        return NULL;
    }

    return heap;
}

/*
 * Toma dos elementos del vector generico y los intercambia de posicion. Es
 * utilizado por la funciones sift_up y sift_down para ir cambiando la
 * posicion del elemento que se esta "hundiendo" o "flotando".
 */
void swap(void** elementos, size_t pos_a, size_t pos_b) {
    void* tmp = elementos[pos_a];
    elementos[pos_a] = elementos[pos_b];
    elementos[pos_b] = tmp;
}

/*
 * Toma el elemento en la posicion inicial indicada y va retrocediendolo en 
 * cuanto a la posicion que tienen el elemento en el vector si el elemento
 * sigue cumpliendo la condicion del heap. En el caso de un heap minimal esta
 * seria la accion "flotar" que lleva a un elemento en el fondo del heap hasta
 * su nivel correcto.
 */
void sift_up(void** elementos, size_t pos_elemento, heap_comparador comparador) {
    if (pos_elemento == 0)
        return;

    size_t pos_padre = (pos_elemento - 1) / 2;

    void* elemento = elementos[pos_elemento];
    void* elemento_padre = elementos[pos_padre];

    int comparacion = comparador(elemento, elemento_padre);
    if (comparacion <= 0) {
        swap(elementos, pos_elemento, pos_padre);
        sift_up(elementos, pos_padre, comparador);
    }
}

/*
 * Toma el elemento en la posicion inicial indicada y va "hundiendolo" en el 
 * heap haciendolo avanzar en su posicion dentro del vector mientras siga
 * cumpliendo la condicion del heap. En el caso de un heap minimal seria ir
 * "hundiendo" el elemento si aun quedan elementos menores que estan por
 * delante en el vector.
 */
void sift_down(void** elementos, size_t pos_elemento, size_t tope, heap_comparador comparador) {
    size_t pos_izquierdo = (pos_elemento * 2) + 1;
    size_t pos_derecho = (pos_elemento * 2) + 2;
    size_t pos_intercambiar = pos_elemento;

    if (pos_izquierdo > tope)
        return;

    if (pos_derecho > tope)
        pos_derecho = pos_izquierdo;

    void* elemento = elementos[pos_elemento];
    void* elemento_izquierdo = elementos[pos_izquierdo];
    void* elemento_derecho = elementos[pos_derecho];

    int comparacion_izq_der = comparador(elemento_izquierdo, elemento_derecho);
    pos_intercambiar = comparacion_izq_der > 0 ? pos_derecho : pos_izquierdo;

    int comparacion = comparador(elemento, elementos[pos_intercambiar]);
    if (comparacion > 0) {
        swap(elementos, pos_elemento, pos_intercambiar);
        sift_down(elementos, pos_intercambiar, tope, comparador);
    }
}

/*
 * Toma un heap binario valido y un elemento a insertar.
 *
 * Posiciona el elemento en la ubicacion correcta de acuardo con el compardor
 * definido al crear el heap. Por la naturaleza del tda heap, lo elementos
 * quedan reoganizados de ser necesario para que se siga cumpliendo la
 * condicion de minimal o maximal del heap.
 *
 * En caso de exito retorna el tamanio del heap o cero en caso de error.
 */
size_t heap_insertar(heap_t* heap, void* elemento) {
    if (!heap)
        return 0;

    if (heap->tamanio >= heap->maximo) {
        size_t nuevo_maximo = heap->maximo * 2;
        void** elementos_aux = realloc(heap->elementos, nuevo_maximo * sizeof(void*));
        if (!elementos_aux)
            return 0;

        heap->elementos = elementos_aux;
        heap->maximo = nuevo_maximo;
    }

    heap->elementos[heap->tamanio++] = elemento;
    sift_up(heap->elementos, heap->tamanio - 1, heap->comparador);

    return heap->tamanio;
}

/*
 * Recibe un heap binario valido.
 *
 * Si el heap tienen al menos un elemento, retorna ese elemento y lo elimina
 * del heap. Luego reorganiza el heap de ser necesario para que siga cumpliendo
 * la condicion de minimal o maximal.
 *
 * En caso de exito retorna el elemento en la raiz, o NULL en caso de error.
 */
void* heap_extraer_raiz(heap_t* heap) {
    if (!heap)
        return NULL;

    if (heap->tamanio == 0)
        return NULL;

    void* elemento_extraido = heap->elementos[0];

    heap->tamanio--;
    heap->elementos[0] = heap->elementos[heap->tamanio];

    if (heap->tamanio > 0)
        sift_down(heap->elementos, 0, heap->tamanio, heap->comparador);

    return elemento_extraido;
}

/*
 * Toma un heap binario valido.
 * Retorna el tamanio del heap o cero en caso de error.
 */
size_t heap_tamanio(heap_t* heap) {
    if (!heap)
        return 0;

    return heap->tamanio;
}

/*
 * Toma un heap binario valido y un posible destrcutor. En caso de que se
 * le pase un destructor, lo utiliza para destruir los elementos restantes
 * en el heap. Finalmente libera la memoria del heap en si.
 */
void heap_destruir(heap_t* heap, void (*destructor)(void*)) {
	if (!heap)
		return;

	if (destructor) {
		while (heap_tamanio(heap) > 0) {
			destructor(heap_extraer_raiz(heap));
		}
	}

	free(heap->elementos);
	free(heap);
}
