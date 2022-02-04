#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospital.h"
#include "parser.h"
#include "split.h"

typedef struct entrenador {
	int   id;
	char* nombre;
} entrenador_t;

struct _hospital_pkm_t {
	size_t        cantidad_pokemon;
	size_t        cantidad_entrenador;
	pokemon_t*    vector_pokemones;
	entrenador_t* vector_entrenadores;
};

struct _pkm_t {
	char*  nombre;
	size_t nivel;
};

hospital_t*
hospital_crear()
{
	return calloc(1, sizeof(hospital_t));
}

/**
  * Crea un nuevo pokemon con el nivel y nombre correspondientes.
  * En caso de error devuelve NULL. Caso contrario, un puntero al nuevo pokemon creado.
  */
pokemon_t*
crear_pokemon(char* nivel, char* nombre)
{
	if (!nombre)
		return NULL;

	pokemon_t* nuevo_pokemon = malloc(sizeof(pokemon_t));
	if (!nuevo_pokemon)
		return NULL;

	nuevo_pokemon -> nivel  = (size_t)atoi(nivel);
	nuevo_pokemon -> nombre = malloc((strlen(nombre) + 1) * sizeof(char));
	if (!nuevo_pokemon -> nombre) {
		free(nuevo_pokemon);
		return NULL;
	}

	strcpy(nuevo_pokemon -> nombre, nombre);
	return nuevo_pokemon;
}

/**
  * Agrega un pokemon dado al vector de pokemones de un hospital (en la ultima posicion).
  * En caso de error no agrega el pokemon dado al hospital.
  */
void
hospital_guardar_pokemon(hospital_t* hospital, pokemon_t* nuevo_pokemon)
{
	if (!hospital || !nuevo_pokemon)
		return;

	size_t cantidad_pokemon = hospital_cantidad_pokemon(hospital);
	pokemon_t* vector_pokemones_aux = realloc(hospital -> vector_pokemones, (cantidad_pokemon + 1) * sizeof(pokemon_t));
	if (!vector_pokemones_aux)
		return;

	hospital -> vector_pokemones = vector_pokemones_aux;
	hospital -> vector_pokemones[hospital -> cantidad_pokemon++] = *nuevo_pokemon;
}

/**
  * Crea un nuevo entrenador con el ID y nombre correspondientes.
  * En caso de error devuelve NULL. Caso contrario, un puntero al nuevo entrenador creado.
  */
entrenador_t*
crear_entrenador(char* id, char* nombre)
{
	if (!nombre)
		return NULL;

	entrenador_t* nuevo_entrenador = malloc(sizeof(entrenador_t));
	if (!nuevo_entrenador)
		return NULL;

	nuevo_entrenador -> id = atoi(id);
	nuevo_entrenador -> nombre = malloc((strlen(nombre) + 1) * sizeof(char));
	if (!nuevo_entrenador -> nombre) {
		free(nuevo_entrenador);
		return NULL;
	}

	strcpy(nuevo_entrenador -> nombre, nombre);
	return nuevo_entrenador;
}

/**
  * Agrega un entrenador dado al vector de pokemones de un hospital (en la ultima posicion).
  * En caso de error no agrega el pokemon dado al hospital.
  */
void
hospital_guardar_entrenador(hospital_t* hospital, entrenador_t* nuevo_entrenador)
{
	if (!hospital || !nuevo_entrenador)
		return;

	size_t cantidad_entrenador = hospital_cantidad_entrenadores(hospital);
	entrenador_t* vector_entrenadores_aux = realloc(hospital -> vector_entrenadores, (cantidad_entrenador + 1) * sizeof(entrenador_t));
	if (!vector_entrenadores_aux)
		return;

	hospital -> vector_entrenadores = vector_entrenadores_aux;
	hospital -> vector_entrenadores[hospital -> cantidad_entrenador++] = *nuevo_entrenador;
}

/**
  * Toma una linea leida del archivo de texto dado, obteniene la informacion de dicha linea
  * y finalmente se encarga de guardar toda la informacion de ese registro en el hospital.
  *
 * Devuelve true si pudo guardarlo o false en caso de error.
  */
bool
hospital_guardar_informacion(hospital_t* hospital, char* linea_archivo)
{
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

	hospital_guardar_entrenador(hospital, nuevo_entrenador);
	free(nuevo_entrenador);

	for (size_t i = 2; informacion_linea[i]; i += 2) {
		pokemon_t* nuevo_pokemon = crear_pokemon(informacion_linea[i + 1], informacion_linea[i]);
		if (!nuevo_pokemon) {
			free_vector_strings(informacion_linea);
			free(informacion_linea);
			return false;
		}

		hospital_guardar_pokemon(hospital, nuevo_pokemon);
		free(nuevo_pokemon);
	}

	free_vector_strings(informacion_linea);
	free(informacion_linea);

	return true;
}

bool
hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo)
{
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

size_t
hospital_cantidad_pokemon(hospital_t* hospital)
{
	if (!hospital)
		return 0;
	return hospital -> cantidad_pokemon;
}

size_t
hospital_cantidad_entrenadores(hospital_t* hospital)
{
	if (!hospital)
		return 0;
	return hospital -> cantidad_entrenador;
}

/**
  * Ordena las referencias a los pokemones para que se les pueda aplicar una funcion
  * en orden alfabético tener que sin modificar el orden original del vector de
  * pokemones almacenados en el hospital.
  *
  * Recibe un vector de referencias a pokemones, que inicialmente comparte el mismo orden
  * que el vector original de pokemones almacenados en el hospital. Además recibe la cantidad
  * de pokemones actual.
  */
void
ordenar_referencia_a_pokemones(pokemon_t** referencias_pokemones, size_t cantidad_pokemon)
{
	if (!referencias_pokemones || !cantidad_pokemon)
		return;

	for (size_t i = 0; i < (cantidad_pokemon - 1); i++) {
		size_t indice_min = i;
		for (size_t j = i + 1; j < cantidad_pokemon; j++) {
			if (strcmp((*referencias_pokemones[j]).nombre, (*referencias_pokemones[indice_min]).nombre) < 0)
				indice_min = j;
		}

		pokemon_t* temp = referencias_pokemones[indice_min];
		referencias_pokemones[indice_min] = referencias_pokemones[i];
		referencias_pokemones[i] = temp;
	}
}

size_t
hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p))
{
	if (!hospital || !funcion)
		return 0;

	size_t cantidad_pokemon = hospital_cantidad_pokemon(hospital);
	pokemon_t** referencias_pokemones = malloc(cantidad_pokemon * sizeof(pokemon_t));
	for (size_t i = 0; i < cantidad_pokemon; i++)
		referencias_pokemones[i] = &(hospital -> vector_pokemones[i]);

	ordenar_referencia_a_pokemones(referencias_pokemones, cantidad_pokemon);

	size_t recorridos = 0;
	while ((recorridos < cantidad_pokemon) && funcion(referencias_pokemones[recorridos++]));

	free(referencias_pokemones);
	return recorridos;
}

void
hospital_destruir(hospital_t* hospital)
{
	if (!hospital)
		return;

	if (hospital -> vector_pokemones) {
		size_t cantidad_pokemon = hospital_cantidad_pokemon(hospital);
		for (size_t i = 0; i < cantidad_pokemon; i++)
			free(hospital -> vector_pokemones[i].nombre);
		
		free(hospital -> vector_pokemones);
	}

	if (hospital -> vector_entrenadores) {
		size_t cantidad_entrenador = hospital_cantidad_entrenadores(hospital);
		for (size_t i = 0; i < cantidad_entrenador; i++)
			free(hospital -> vector_entrenadores[i].nombre);
		
		free(hospital -> vector_entrenadores);
	}

	if (hospital)
		free(hospital);
}

size_t
pokemon_nivel(pokemon_t* pokemon)
{
	if (!pokemon)
		return 0;
	return pokemon -> nivel;
}

const char*
pokemon_nombre(pokemon_t* pokemon)
{
	if (!pokemon)
		return NULL;
	return pokemon -> nombre;
}
