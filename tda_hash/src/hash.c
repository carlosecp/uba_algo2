#include "hash.h"

#include <stdio.h>
#include <stdlib.h>

#include "casilla.h"

#define CAPACIDAD_MINIMA 3
#define UMBRAL_REHASH 0.75
#define MULTIPLICADOR_CANTIDAD_REHASH 2;

struct hash {
    casilla_t** casillas;
    size_t cantidad_casillas;
    size_t cantidad_elementos;
    hash_destruir_dato_t destruir_elemento;
};

void hash_destruir_aux(hash_t tmp);

hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad_inicial) {
    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash)
        return NULL;

    hash->cantidad_casillas = capacidad_inicial >= CAPACIDAD_MINIMA ? capacidad_inicial : CAPACIDAD_MINIMA;
    hash->cantidad_elementos = 0;
    hash->destruir_elemento = destruir_elemento;

    hash->casillas = calloc(hash->cantidad_casillas, sizeof(casilla_t*));
    if (!(hash->casillas)) {
        free(hash);
        return NULL;
    }

    return hash;
}

double calcular_factor_de_carga(size_t cantidad_elementos, size_t cantidad_casillas) {
    if (!cantidad_casillas)
        return ERROR;

    return (double)cantidad_elementos / (double)cantidad_casillas;
}

void rehash(hash_t* hash) {
	if (!hash)
		return;

	hash_t tmp = *hash;

	hash->cantidad_casillas *= MULTIPLICADOR_CANTIDAD_REHASH;
	hash->casillas = calloc(hash->cantidad_casillas, sizeof(casilla_t*));
	hash->cantidad_elementos = 0;

	for (size_t i = 0; i < tmp.cantidad_casillas; i++)
		casilla_copiar_casillas(tmp.casillas[i], hash);

	tmp.destruir_elemento = NULL;
	hash_destruir_aux(tmp);
}

int calcular_indice_hash(const char* clave) {
    if (!clave)
        return ERROR;

    size_t indice_hash = 0;
    while (*clave)
		indice_hash += *(clave++);

    return indice_hash;
}

int hash_insertar(hash_t* hash, const char* clave, void* elemento) {
	if (!hash || !clave)
		return ERROR;

	double factor_de_carga = calcular_factor_de_carga(hash->cantidad_elementos, hash->cantidad_casillas);
	if ((int)factor_de_carga == ERROR)
		return ERROR;

	if (factor_de_carga >= UMBRAL_REHASH)
		rehash(hash);

	int indice_clave = calcular_indice_hash(clave);
	if (indice_clave == ERROR)
		return ERROR;

	indice_clave %= hash->cantidad_casillas;
	casilla_t* tmp = casilla_insertar(hash->casillas[indice_clave], clave, elemento, hash->destruir_elemento, &(hash->cantidad_elementos));
	if (!tmp)
		return ERROR;

	hash->casillas[indice_clave] = tmp;
	return EXITO;
}

int hash_quitar(hash_t* hash, const char* clave) {
    if (!hash || !clave)
        return ERROR;

    int indice_clave = calcular_indice_hash(clave);
    if (indice_clave == ERROR)
        return ERROR;

    indice_clave %= hash->cantidad_casillas;
    return casilla_quitar(&(hash->casillas[indice_clave]), clave, hash->destruir_elemento, &(hash->cantidad_elementos));
}

void* hash_obtener(hash_t* hash, const char* clave) {
    if (!hash || !clave)
        return NULL;

    int indice_clave = calcular_indice_hash(clave);
    if (indice_clave == ERROR)
        return NULL;

    indice_clave %= hash->cantidad_casillas;
    return casilla_obtener(hash->casillas[indice_clave], clave);
}

bool hash_contiene(hash_t* hash, const char* clave) {
    if (!hash || !clave)
        return false;

    int indice_clave = calcular_indice_hash(clave);
    if (indice_clave == ERROR)
        return false;

    indice_clave %= hash->cantidad_casillas;
    void* elemento = casilla_obtener(hash->casillas[indice_clave], clave);
    return elemento ? true : false;
}

size_t hash_cantidad(hash_t* hash) {
    if (!hash)
        return 0;

    return hash->cantidad_elementos;
}

void hash_destruir(hash_t* hash) {
    if (!hash)
        return;

    for (size_t i = 0; i < hash->cantidad_casillas; i++)
        casilla_destruir(hash->casillas[i], hash->destruir_elemento);

    free(hash->casillas);
    free(hash);
}

void hash_destruir_aux(hash_t tmp) {
    for (size_t i = 0; i < tmp.cantidad_casillas; i++)
        casilla_destruir(tmp.casillas[i], NULL);

    free(tmp.casillas);
}

size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux) {
	if (!hash)
		return 0;

	size_t cantidad_recorridos = 0;
	size_t detener_recorrido = false;

	for (size_t i = 0; i < hash->cantidad_casillas && !detener_recorrido; i++)
		detener_recorrido = casilla_con_cada_clave(hash->casillas[i], hash, funcion, aux, &cantidad_recorridos);

	return cantidad_recorridos;
}
