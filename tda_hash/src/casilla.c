#include "casilla.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

casilla_t* casilla_crear() {
    return NULL;
}

casilla_t* casilla_insertar(casilla_t* casilla, const char* clave, void* elemento, hash_destruir_dato_t destruir_elemento, size_t* cantidad_elementos) {
	if (!clave || !cantidad_elementos) {
		return NULL;
	}

    if (!casilla) {
        casilla_t* nueva_casilla = malloc(sizeof(casilla_t));
        if (!nueva_casilla)
            return NULL;

		char* copia_clave = malloc(sizeof(strlen(clave)));
		if (!copia_clave)
			return NULL;

		nueva_casilla->clave = copia_clave;
		strcpy(nueva_casilla->clave, clave);

        nueva_casilla->elemento = elemento;
        nueva_casilla->siguiente = NULL;

        (*cantidad_elementos)++;

        return nueva_casilla;
    }

    if (strcmp(casilla->clave, clave) == 0) {
        if (destruir_elemento)
			destruir_elemento(casilla->elemento);

        casilla->elemento = elemento;
        return casilla;
    } else {
        casilla->siguiente = casilla_insertar(casilla->siguiente, clave, elemento, destruir_elemento, cantidad_elementos);
    }

    return casilla;
}

int casilla_quitar(casilla_t** casilla, const char* clave, hash_destruir_dato_t destruir_elemento, size_t* cantidad_elementos) {
    if (!casilla || !clave)
        return ERROR;

    while (*casilla && strcmp((*casilla)->clave, clave))
        casilla = &((*casilla)->siguiente);

    if (*casilla) {
        if (destruir_elemento)
            destruir_elemento((*casilla)->elemento);

        casilla_t* tmp = *casilla;
        *casilla = (*casilla)->siguiente;
		free(tmp->clave);
        free(tmp);

        (*cantidad_elementos)--;
        return EXITO;
    }

    return ERROR;
}

void* casilla_obtener(casilla_t* casilla, const char* clave) {
    if (!casilla || !clave)
        return NULL;

    if (strcmp(casilla->clave, clave) == 0)
        return casilla->elemento;

    return casilla_obtener(casilla->siguiente, clave);
}

void casilla_destruir(casilla_t* casilla, hash_destruir_dato_t destruir_elemento) {
    if (!casilla)
        return;

    if (destruir_elemento)
        destruir_elemento(casilla->elemento);

    casilla_t* siguiente = casilla->siguiente;
	free(casilla->clave);
    free(casilla);

    casilla_destruir(siguiente, destruir_elemento);
}

bool casilla_con_cada_clave(casilla_t* casilla, hash_t* hash, bool (*funcion)(hash_t*, const char*, void*), void* aux, size_t* cantidad_recorridos) {
	if (!casilla || !cantidad_recorridos)
		return false;

	(*cantidad_recorridos)++;

	bool detener_recorrido = false;

	if (funcion)
		detener_recorrido = funcion(hash, casilla->clave, aux);

	if (!detener_recorrido)
		detener_recorrido = casilla_con_cada_clave(casilla->siguiente, hash, funcion, aux, cantidad_recorridos);

	return detener_recorrido;
}

void casilla_copiar_casillas(casilla_t* casilla, hash_t* hash) {
	if (!casilla || !hash)
		return;

	hash_insertar(hash, casilla->clave, casilla->elemento);

	casilla_copiar_casillas(casilla->siguiente, hash);
}
