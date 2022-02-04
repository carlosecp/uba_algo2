#ifndef AUX_SIMULADOR_ATENCION_POKEMON_H__
#define AUX_SIMULADOR_ATENCION_POKEMON_H__

#include <stddef.h>

#include "auxiliares_hospital.h"
#include "heap.h"
#include "hospital.h"
#include "lista.h"
#include "simulador.h"

typedef struct {
    char* nombre_pokemon;
    char* nombre_entrenador;
    size_t nivel;
} PokemonEnRecepcion;

/**
 * Comparador de pokemones en base a su nivel. Es utilizado por el heap que
 * almacena los pokemones en la recepcion para que el mantenga a estos
 * pokemones ordenados de tal forma que el pokemon de menor nivel siempre este
 * de primero en el heap (en la raiz).
 */
int aux_comparador_nivel_pokemon(void* _pokemon_a, void* _pokemon_b);

/**
 * Toma un pokemon directamente desde el hospital y crea una nueva estructura
 * en la que se almacena la informacion del pokemon junto con el nombre de
 * su entrenador. Realiza una copia de los valores de los nombres para prevenir
 * errores si estos valores son cambiados por el usuario.
 */
PokemonEnRecepcion* aux_asociar_pokemon_con_su_entrenador(pokemon_t* pokemon, char* nombre_entrenador);

/**
 * Toma el entrenador pokemon que esta ingresando en el hospital y pasa a todos
 * sus pokemones al area de recepcion para que esperen a ser atendidos. Ademas
 * toma un iterador de los pokemones en el hospital del cual va a ir extrayendo
 * los pokemones correspondientes al entrenador actual en base a la cantidad
 * de pokemones del entrenador.
 */
bool aux_agregar_pokemones_de_entrenador_a_recepcion(entrenador_t* proximo_entrenador, lista_iterador_t* sala_de_espera_pokemones, heap_t* recepcion);

/**
 * Destructor de pokemones almacenados en el heap, para los pokemones que
 * no pasaron a ser atendidos si finaliza la ejecucion del juego o tambien
 * utilizado para liberar de memoria a un pokemon que ya ha sido atentido.
 */
void aux_destruir_pokemon_en_recepcion(void* _pokemon);

#endif
