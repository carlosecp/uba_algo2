#ifndef AUXILIARES_HOSPITAL_H__
#define AUXILIARES_HOSPITAL_H__

#include "abb.h"
#include "lista.h"

struct _hospital_pkm_t {
	lista_t* entrenadores;
	lista_t* pokemones_orden_llegada;
	abb_t* pokemones_orden_alfabetico;
};

struct _entrenador_t {
	int id;
	char* nombre;
	size_t cantidad_pokemones;
};

struct _pkm_t {
	char* nombre;
	size_t nivel;
};

#endif
