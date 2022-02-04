#include "hospital.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auxiliares_hospital.h"
#include "parser_hospital.h"
#include "split.h"

/**
 * Destructor utilizado para liberar un entrenador de memoria cuando se
 * elimina del hospital.
 */
bool destruir_entrenador(void* _entrenador, void* aux) {
	entrenador_t* entrenador = _entrenador;

	free(entrenador->nombre);
	free(entrenador);

	return true;
}

/**
 * Destructor utilizado para liberar un pokemon de memoria cuando se
 * elimina del hospital.
 */
void destruir_pokemon(void* _pokemon) {
	pokemon_t* pokemon = _pokemon;
	free(pokemon->nombre);
	free(pokemon);
}

/*
 * Comparador de pokemones en base al nombre de cada uno utlizado por el abb
 * que almacena los pokemones ordenados de manera alfabetica.
 */
int abb_comparador_pokemones_por_nombre(void* _p1, void* _p2) {
	pokemon_t* p1 = _p1;
	pokemon_t* p2 = _p2;
	return strcmp(p1->nombre, p2->nombre);
}

typedef struct {
	bool (*funcion)(pokemon_t* p);
} wrapper_funcion_aux;

/**
 * Funcion auxiliar pasada para el recorrido del arbol binario de busqueda que
 * almacena a los pokemones en orden alfabetico. Esta funcion toma como
 * auxiliar un puntero del tipo wrapper_funcion_aux debido a que la firma del
 * iterador de abb no concuerda con la firma de la funcion que se llama sobre
 * cada pokemon.
 */
bool funcion_aux(void* _pokemon, void* _aux) {
	pokemon_t* pokemon = _pokemon;
	wrapper_funcion_aux* aux_funcion = _aux;
	return aux_funcion->funcion(pokemon);
}

hospital_t* hospital_crear() {
	hospital_t* hospital = calloc(1, sizeof(hospital_t));
	if (!hospital)
		return NULL;

	hospital->entrenadores = lista_crear();
	if (!(hospital->entrenadores)) {
		free(hospital);
		return NULL;
	}

	hospital->pokemones_orden_llegada = lista_crear();
	if (!(hospital->pokemones_orden_llegada)) {
		lista_destruir(hospital->entrenadores);
		free(hospital);
		return NULL;
	}

	hospital->pokemones_orden_alfabetico = abb_crear(abb_comparador_pokemones_por_nombre);
	if (!hospital->pokemones_orden_alfabetico) {
		lista_destruir(hospital->pokemones_orden_llegada);
		lista_destruir(hospital->entrenadores);
		free(hospital);
		return NULL;
	}

	return hospital;
}

/**
 * Crea un nuevo pokemon con el nivel y nombre correspondientes.
 * En caso de error devuelve NULL. Caso contrario, un puntero al nuevo pokemon creado.
 */
pokemon_t* crear_pokemon(char* nivel, char* nombre) {
	if (!nombre)
		return NULL;

	pokemon_t* nuevo_pokemon = malloc(sizeof(pokemon_t));
	if (!nuevo_pokemon)
		return NULL;

	nuevo_pokemon->nivel = (size_t)atoi(nivel);
	nuevo_pokemon->nombre = malloc((strlen(nombre) + 1) * sizeof(char));
	if (!nuevo_pokemon->nombre) {
		free(nuevo_pokemon);
		return NULL;
	}

	strcpy(nuevo_pokemon->nombre, nombre);
	return nuevo_pokemon;
}

/**
 * Crea un nuevo entrenador con el ID y nombre correspondientes.
 * En caso de error devuelve NULL. Caso contrario, un puntero al nuevo entrenador creado.
 */
entrenador_t* crear_entrenador(char* id, char* nombre) {
	if (!nombre)
		return NULL;

	entrenador_t* nuevo_entrenador = malloc(sizeof(entrenador_t));
	if (!nuevo_entrenador)
		return NULL;

	nuevo_entrenador->id = atoi(id);
	nuevo_entrenador->nombre = malloc((strlen(nombre) + 1) * sizeof(char));
	if (!nuevo_entrenador->nombre) {
		free(nuevo_entrenador);
		return NULL;
	}

	strcpy(nuevo_entrenador->nombre, nombre);

	nuevo_entrenador->cantidad_pokemones = 0;

	return nuevo_entrenador;
}

/**
 * Toma una linea leida del archivo de texto dado, obteniene la informacion de dicha linea
 * y finalmente se encarga de guardar toda la informacion de ese registro en el hospital.
 *
 * Devuelve true si pudo guardarlo o false en caso de error.
 */
bool hospital_guardar_informacion(hospital_t* hospital, char* linea_archivo) {
	if (!hospital || !linea_archivo)
		return false;

	char** informacion_linea = obtener_informacion_linea(linea_archivo);
	if (!informacion_linea)
		return false;

	entrenador_t* nuevo_entrenador = crear_entrenador(informacion_linea[0], informacion_linea[1]);
	if (!nuevo_entrenador) {
		free(informacion_linea);
		return false;
	}

	for (size_t i = 2; informacion_linea[i]; i += 2) {
		pokemon_t* nuevo_pokemon = crear_pokemon(informacion_linea[i + 1], informacion_linea[i]);
		if (!nuevo_pokemon) {
			free_vector_strings(informacion_linea);
			free(informacion_linea);
			return false;
		}

		lista_t* pokemones_orden_llegada_aux = lista_insertar(hospital->pokemones_orden_llegada, nuevo_pokemon);
		abb_t* pokemones_orden_alfabetico_aux = abb_insertar(hospital->pokemones_orden_alfabetico, nuevo_pokemon);

		if (!pokemones_orden_llegada_aux || !pokemones_orden_alfabetico_aux) {
			destruir_pokemon(nuevo_pokemon);
			free_vector_strings(informacion_linea);
			free(informacion_linea);
			return false;
		}

		hospital->pokemones_orden_llegada = pokemones_orden_llegada_aux;
		hospital->pokemones_orden_alfabetico = pokemones_orden_alfabetico_aux;

		nuevo_entrenador->cantidad_pokemones++;
	}

	hospital->entrenadores = lista_insertar(hospital->entrenadores, nuevo_entrenador);

	free_vector_strings(informacion_linea);
	free(informacion_linea);

	return true;
}

bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo) {
	if (!hospital)
		return false;

	FILE* archivo = fopen(nombre_archivo, "r");
	if (!archivo)
		return false;

	char* linea = leer_linea(archivo);
	bool continuar_guardando_informacion = true;

	while (linea && continuar_guardando_informacion) {
		continuar_guardando_informacion = hospital_guardar_informacion(hospital, linea);
		free(linea);
		linea = leer_linea(archivo);
	}

	fclose(archivo);
	return true;
}

size_t hospital_cantidad_pokemon(hospital_t* hospital) {
	if (!hospital)
		return 0;

	return abb_tamanio(hospital->pokemones_orden_alfabetico);
}

size_t hospital_cantidad_entrenadores(hospital_t* hospital) {
	if (!hospital)
		return 0;

	return lista_tamanio(hospital->entrenadores);
}

size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)) {
	if (!hospital || !funcion)
		return 0;

	wrapper_funcion_aux aux = {funcion};
	size_t cantidad_recorridos = abb_con_cada_elemento(hospital->pokemones_orden_alfabetico, INORDEN, funcion_aux, &aux);
	return cantidad_recorridos;
}

void hospital_destruir(hospital_t* hospital) {
	if (!hospital)
		return;

	lista_con_cada_elemento(hospital->pokemones_orden_llegada, NULL, NULL);
	lista_con_cada_elemento(hospital->entrenadores, destruir_entrenador, NULL);

	lista_destruir(hospital->pokemones_orden_llegada);
	lista_destruir(hospital->entrenadores);

	abb_destruir_todo(hospital->pokemones_orden_alfabetico, destruir_pokemon);

	free(hospital);
}

size_t pokemon_nivel(pokemon_t* pokemon) {
	if (!pokemon)
		return 0;
	return pokemon->nivel;
}

const char* pokemon_nombre(pokemon_t* pokemon) {
	if (!pokemon)
		return NULL;
	return pokemon->nombre;
}
