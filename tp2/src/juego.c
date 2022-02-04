#include "juego.h"

#include <stdio.h>

#include "simulador.h"

#define PUNTAJE_INICIAL 100

/**
 * Funcion auxiliar utilizada para verificar el nivel de las dos dificultades
 * agregadas al iniciar el juego: "nightmare" y "ultra_nightmare". Ambas
 * dificultades comparten este mismo verificador.
 */
int juego_verificar_nivel(unsigned nivel_adivinado, unsigned nivel_pokemon) {
	return (int)nivel_pokemon - (int)nivel_adivinado;
}

/**
 * Funcion calcular_puntaje utilizada en la dificultad "nightmare".
 * Calcula el puntaje final en base a la division del puntaje inicial entre
 * la cantidad de intentos.
 */
unsigned calcular_puntaje_nightmare(unsigned cantidad_intentos) {
	return PUNTAJE_INICIAL / cantidad_intentos + 1;
}

/**
 * Funcion verificacion_a_string que genera una string que describe el
 * resultado de un intento de adivinar el nivel del pokemon en tratamiento
 * para la dificultad adicional "nightmare".
 */
const char* verificacion_a_string_nightmare(int resultado_verificacion) {
	if (resultado_verificacion == 0)
		return "Adivinaste Crack";

	return "No Adivinaste Crack";
}

/**
 * Funcion auxiliar que prepara los datos de la dificultad adicional "nightmare"
 * para que esta sea agregada en el simulador cuando se inicialice el juego.
 */
DatosDificultad datos_dificultad_nightmare() {
	return (DatosDificultad){
		.nombre = "Nightmare",
			.calcular_puntaje = calcular_puntaje_nightmare,
			.verificar_nivel = juego_verificar_nivel,
			.verificacion_a_string = verificacion_a_string_nightmare,
	};
}

/**
 * Funcion calcular_puntaje utilizada en la dificultad "ultra_nightmare".
 * Calcula el puntaje final en base a la division del puntaje inicial
 * menos la cantidad de intentos, entre la cantidad de intentos.
 */
unsigned calcular_puntaje_ultra_nightmare(unsigned cantidad_intentos) {
	return PUNTAJE_INICIAL - cantidad_intentos / cantidad_intentos + 1;
}

/**
 * Funcion verificacion_a_string que genera una string que describe el
 * resultado de un intento de adivinar el nivel del pokemon en tratamiento
 * para la dificultad adicional "ultra_nightmare".
 */
const char* verificacion_a_string_ultra_nightmare(int resultado_verificacion) {
	if (resultado_verificacion == 0)
		return "Adivinaste Crack";

	int pista_aleatoria = rand() % 5;

	switch (pista_aleatoria) {
		case 0:
			return "Diego Eterno";
		case 1:
			return "F";
		case 2:
			return "Casi";
		default:
			return "Grande Boca";
	}
}

/**
 * Funcion auxiliar que prepara los datos de la dificultad adicional
 * "ultra_nightmare" para que esta sea agregada en el simulador cuando se
 * inicialice el juego.
 */
DatosDificultad datos_dificultad_ultra_nightmare() {
	return (DatosDificultad){
		.nombre = "Ultra Nightmare",
			.calcular_puntaje = calcular_puntaje_ultra_nightmare,
			.verificar_nivel = juego_verificar_nivel,
			.verificacion_a_string = verificacion_a_string_ultra_nightmare,
	};
}

void juego_inicializar(Juego* juego, hospital_t* hospital) {
	if (!juego || !hospital)
		return;

	juego->simulador = simulador_crear(hospital);
	juego->jugando = true;

	DatosDificultad dificultad_nightmare = datos_dificultad_nightmare();
	DatosDificultad dificultad_ultra_nightmare = datos_dificultad_ultra_nightmare();

	simulador_simular_evento(juego->simulador, AgregarDificultad, &dificultad_nightmare);
	simulador_simular_evento(juego->simulador, AgregarDificultad, &dificultad_ultra_nightmare);
}

void juego_titulo(const char* titulo) {
	printf("\x1b[38;5;0m\x1b[48;5;42m\x1b[1m %s \x1b[0m\n", titulo);
}

void juego_prompt_exito(const char* prompt, const char* mensaje) {
	printf("\x1b[38;5;42m\x1b[1m%s\x1b[0m: %s\n", prompt, mensaje);
}

void juego_prompt_error(const char* prompt, const char* mensaje) {
	printf("\x1b[38;5;9m\x1b[1m%s\x1b[0m: %s\n", prompt, mensaje);
}

void juego_input_prompt(const char* prompt) {
	printf("\x1b[1m%s:\x1b[0m ", prompt);
}

void juego_terminar(Juego* juego) {
	if (!juego)
		return;

	juego->jugando = false;
}

void juego_destruir(Juego juego) {
	simulador_destruir(juego.simulador);
}
