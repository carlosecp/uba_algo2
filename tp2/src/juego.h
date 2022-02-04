#ifndef JUEGO_H_
#define JUEGO_H_

#include <stdbool.h>

#include "hospital.h"
#include "simulador.h"

typedef struct {
    simulador_t* simulador;
    bool jugando;
} Juego;

/**
 * Toma un hospital valido e inicializa el juego y su simulador. Durante la
 * inicializacion tambien se crean dos dificultades adicionales.
 */
void juego_inicializar(Juego* juego, hospital_t* hospital);

/**
 * Funcion auxiliar utilizada para estandarizar la forma en la que se muestran
 * los titulos en la interfaz. Se consideran como titulo los mensajes que se
 * quieran que tengan la mayor notoriedad posible.
 */
void juego_titulo(const char* titulo);

/**
 * Funcion auxiliar utilizada para estandarizar la forma en la que se muestran
 * los mensajes de exito. Estos mensajes no tienen tanta notoriedad como los
 * titulos.
 */
void juego_prompt_exito(const char* prompt, const char* mensaje);

/**
 * Funcion auxiliar utilizada para estandarizar la forma en la que se muestran
 * los mensajes de error. Estos mensajes no tienen tanta notoriedad como los
 * titulos.
 */
void juego_prompt_error(const char* prompt, const char* mensaje);

/**
 * Funcion auxiliar utilizada para estandarizar la forma en la que se muestran
 * los mensaje para indicarle al usuario que tiene que ingresar input.
 */
void juego_input_prompt(const char* prompt);

/**
 * Recibe un juego inicalizado.
 * Da un juego por terminado.
 */
void juego_terminar(Juego* juego);

/**
 * Recibe un juego.
 * Se encarga de la destruccion del simulador y la limpieza de todos los
 * datos que sean necesarios.
 */
void juego_destruir(Juego juego);

#endif
