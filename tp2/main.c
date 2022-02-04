#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "src/hospital.h"
#include "src/juego.h"
#include "src/simulador.h"

char leer_comando() {
	char linea[100];
	char* leido;
	leido = fgets(linea, 100, stdin);
	if (!leido)
		return 0;
	while (*leido == ' ')
		leido++;
	return (char)tolower(*leido);
}

/**
 * Muestra un menu con los posibles comandos que pueden ser ejecutados en el
 * juego. Se muestra una sola vez al iniciar la partida.
 */
void juego_mostrar_menu() {
	juego_titulo("Menu");
	printf(" • [e] Mostrar Estadisticas\n");
	printf(" • [p] Atender Proximo Entrenador\n");
	printf(" • [i] Mostrar Pokemon en Tratamiento\n");
	printf(" • [a] Adivinar Nivel Pokemon\n");
	printf(" • [o] Mostrar Dificultades Disponibles\n");
	printf(" • [d] Cambiar Dificultad\n");
	printf(" • [h] Ayuda\n");
	printf(" • [q] Salir\n");
}

/**
 * Muestra las mismas opciones que el menu pero con mas informacion sobre que
 * hace cada uno de los comandos disponibles. Este menu de ayuda puede ser
 * mostrado en cualquier momento del juego.
 */
void juego_mostrar_ayuda() {
	juego_titulo("Ayuda");
	printf(" • \x1b[1m[e] Mostrar Estadisticas:\x1b[0m Echa un vistazo al estado actual de la partida y los datos sobre entrenadores y pokemones disponibles.\n");
	printf(" • \x1b[1m[p] Atender Proximo Entrenador:\x1b[0m Agrega los pokemones del siguiente entrenador en espera al consultorio.\n");
	printf(" • \x1b[1m[i] Mostrar Pokemon en Tratamiento:\x1b[0m Muestra los datos del pokemon que esta en tratamiento actualmente.\n");
	printf(" • \x1b[1m[a] Adivinar Nivel Pokemon:\x1b[0m Intenta ver si puedes adivinar el nivel del pokemon en tratamiento con la dificultad actual.\n");
	printf(" • \x1b[1m[o] Mostrar Dificultades Disponibles:\x1b[0m Muestra un listado de todas las dificultades disponibles con sus IDs, y cual es la dificultad activa.\n");
	printf(" • \x1b[1m[d] Cambiar Dificultad:\x1b[0m ¿Te parece muy facil la dificultad actual? Prueba a ver si aun puedes con otras dificultades.\n");
	printf(" • \x1b[1m[h] Ayuda:\x1b[0m Muestra este menu de ayuda.\n");
	printf(" • \x1b[1m[q] Salir:\x1b[0m Termina el juego.\n");
}

/**
 * Se encarga de llamar al evento "ObtenerEstadisticas" y muestra los
 * resultados obtenidos por pantalla. En caso de error no muestra nada.
 */
void juego_mostrar_estadisticas(simulador_t* simulador) {
	EstadisticasSimulacion e;
	ResultadoSimulacion res = simulador_simular_evento(simulador, ObtenerEstadisticas, &e);

	if (res == ErrorSimulacion)
		return;

	juego_titulo("Estadisticas");
	printf(" • Entrenadores Atendidos: %u\n", e.entrenadores_atendidos);
	printf(" • Entrenadores Totales: %u\n", e.entrenadores_totales);
	printf(" • Pokemon Atendidos: %u\n", e.pokemon_atendidos);
	printf(" • Pokemon En Espera: %u\n", e.pokemon_en_espera);
	printf(" • Pokemon Totales: %u\n", e.pokemon_totales);
	printf(" • Puntos: %u\n", e.puntos);
	printf(" • Cantidad Eventos Simulados: %u\n", e.cantidad_eventos_simulados);
}

/**
 * Se encarga de llamar al evento "AtenderProximoEntrenador".
 */
void juego_atender_proximo_entreandor(simulador_t* simulador) {
	simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
}

/**
 * Se encarga de llamar al evento "ObtenerInformacionPokemonEnTratamiento".
 * En caso de que haya un pokemon en tratamiento, muestra la informacion de
 * ese pokemon.
 */
void juego_mostrar_pokemon_en_tratamiento(simulador_t* simulador) {
	InformacionPokemon i = {};
	simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &i);

	juego_titulo("Pokemon en Tratamiento");
	printf(" • Pokemon: %s\n", i.nombre_pokemon);
	printf(" • Entrenador: %s\n", i.nombre_entrenador);
}

/**
 * Se encarga de llamar al evento "AdivinarNivelPokemon". Se le pide al
 * usuario que ingrese un numero correspondiente al nivel del pokemon que
 * actualmente esta en tratamiento. En caso que no haber adivinado, se sigue
 * pidiendo el ingreso de un numero hasta dar con el nivel correcto. Cuando
 * se adivina o se erra el nivel del pokemon se muestra la pista
 * correspondiente.
 */
void juego_adivinar_nivel_pokemon_en_tratamiento(simulador_t* simulador) {
	if (!simulador)
		return;

	Intento intento = {
		.nivel_adivinado = 0,
		.es_correcto = false,
		.resultado_string = "",
	};

	while (!intento.es_correcto) {
		juego_input_prompt("Nivel Pokemon");
		scanf(" %i", &(intento.nivel_adivinado));

		ResultadoSimulacion res = simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
		if (res == ErrorSimulacion)
			return;

		if (!intento.es_correcto)
			juego_prompt_error("Incorrecto", intento.resultado_string);
	}

	juego_prompt_exito("Correcto", intento.resultado_string);
}

/**
 * Se encarga de llamar al evento "SeleccionarDificultad". Le permite al
 * usuario cambiar la dificultad de las pistas que se dan al intentar adivinar
 * el nivel de un pokemon.
 */
void juego_seleccionar_nueva_dificultad(simulador_t* simulador) {
	if (!simulador)
		return;

	int id_nueva_dificultad;

	juego_input_prompt("ID Dificultad");
	scanf(" %i", &id_nueva_dificultad);

	ResultadoSimulacion res = simulador_simular_evento(simulador, SeleccionarDificultad, &id_nueva_dificultad);
	if (res == ErrorSimulacion)
		return;

	InformacionDificultad informacion_nueva_dificultad = {
		.id = id_nueva_dificultad,
	};

	res = simulador_simular_evento(simulador, ObtenerInformacionDificultad, &informacion_nueva_dificultad);
	if (res == ErrorSimulacion)
		return;

	juego_prompt_exito("Dificultad Seleccionada", informacion_nueva_dificultad.nombre_dificultad);
}

/**
 * Se encarga de llamar al evento "ObtenerInformacionDificultad" para todas
 * las dificultades disponibles o hasta que el evento retorne error. Muestra
 * en pantalla las dificultades y si cual es la dificultad activa actualmente.
 */
void juego_mostrar_informacion_dificultad(simulador_t* simulador) {
	InformacionDificultad dificultad_buscada = {
		.nombre_dificultad = NULL,
		.en_uso = false,
		.id = 0,
	};

	ResultadoSimulacion res = simulador_simular_evento(simulador, ObtenerInformacionDificultad, &dificultad_buscada);
	if (res == ErrorSimulacion)
		return;

	juego_titulo("Dificultades");

	while (res == ExitoSimulacion) {
		if (!dificultad_buscada.nombre_dificultad)
			return;

		if (dificultad_buscada.en_uso)
			printf(" • \x1b[38;5;42m\x1b[1m\x1b[4m[%i] %s (en uso)\x1b[0m\n", dificultad_buscada.id, dificultad_buscada.nombre_dificultad);
		else
			printf(" • [%i] %s\n", dificultad_buscada.id, dificultad_buscada.nombre_dificultad);

		dificultad_buscada.id++;

		res = simulador_simular_evento(simulador, ObtenerInformacionDificultad, &dificultad_buscada);
	}
}

/**
 * Recibe un juego inicializado.
 * Toma un comando y ejecuta el evento correspondiente.
 */
void juego_ejecutar_comando(Juego* juego, char comando) {
	simulador_t* simulador = juego->simulador;

	switch (comando) {
		case 'e':
			juego_mostrar_estadisticas(simulador);
			break;
		case 'p':
			juego_atender_proximo_entreandor(simulador);
			break;
		case 'i':
			juego_mostrar_pokemon_en_tratamiento(simulador);
			break;
		case 'a':
			juego_adivinar_nivel_pokemon_en_tratamiento(simulador);
			break;
		case 'd':
			juego_seleccionar_nueva_dificultad(simulador);
			break;
		case 'o':
			juego_mostrar_informacion_dificultad(simulador);
			break;
		case 'h':
			juego_mostrar_ayuda();
			break;
		case 'q':
			juego_terminar(juego);
			break;
		default:
			break;
	}
}

int main(int argc, char* argv[]) {
	hospital_t* hospital = hospital_crear();
	hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

	Juego juego;
	srand((unsigned)time(NULL));
	juego_inicializar(&juego, hospital);

	juego_mostrar_menu();

	do {
		printf("> ");
		char c = leer_comando();
		juego_ejecutar_comando(&juego, c);
	} while (juego.jugando);

	juego_destruir(juego);

	return 0;
}
