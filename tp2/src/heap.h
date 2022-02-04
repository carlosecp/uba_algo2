#ifndef HEAP_H__
#define HEAP_H__

#include <stdio.h>

typedef int (*heap_comparador)(void*, void*);

// Heap Minimal
typedef struct _heap_t {
	void** elementos;
	size_t tamanio;
	size_t maximo;
	heap_comparador comparador;
} heap_t;

heap_t* heap_crear(heap_comparador comparador);

size_t heap_insertar(heap_t* heap, void* elemento);

void* heap_elemento_en_raiz(heap_t* heap);

void* heap_extraer_raiz(heap_t* heap);

void heapify(void** elementos, size_t cantidad);

size_t heap_tamanio(heap_t* heap);

void heap_destruir(heap_t* heap, void (*destructor)(void*));

#endif
