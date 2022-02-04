#include "simulador.h"

#include <stdio.h>
#include <string.h>

#include "aux_simulador_atencion_pokemon.h"
#include "aux_simulador_dificultades.h"
#include "heap.h"
#include "hospital.h"

#define CANTIDAD_DIFICULTADES_INICIAL 3
#define RESULTADO_CORRECTO 0

struct _simulador_t {
	hospital_t* hospital;
	EstadisticasSimulacion estadisticas;

	heap_t* recepcion;
	lista_iterador_t* sala_espera_entrenadores;
	lista_iterador_t* sala_espera_pokemones;
	PokemonEnRecepcion* pokemon_en_tratamiento;

	abb_t* dificultades;
	DatosDificultadConId dificultad_en_uso;
	unsigned intentos_actuales;

	bool en_curso;
};

/**
 * Actualiza la el pokemon en tratamiento cuando se agregan los primeros
 * pokemones al simulador o cuando se ha adivinado el nivel del pokemon que
 * estaba anteriormente en tratamiento. Siempre pasa al pokemon de menor
 * nivel disponible. En caso de que no haya ningun pokemon actualmente en 
 * tratamiento, igual agrega el proximo pokemon de menor nivel en la recepcion
 * como nuevo pokemon en tratamiento. Esto debido a que la funcion auxiliar de
 * destruccion tambien previene estos casos.
 *
 * En caso de error no hace ningun cambio.
 */
void simulador_atender_pokemon_de_menor_nivel(simulador_t* simulador) {
	if (!simulador)
		return;

	PokemonEnRecepcion* pokemon = simulador->pokemon_en_tratamiento;
	simulador->pokemon_en_tratamiento = heap_extraer_raiz(simulador->recepcion);
	aux_destruir_pokemon_en_recepcion(pokemon);
}

/**
 * Recibe un simulador con todas sus estructuras de datos inicializadas por sus
 * respetivos constructores. En caso contrario se esta enviando memoria
 * invalida, lo que ocasiona un error en la liberacion.
 *
 * Libera la memoria de las estructuras de datos que componen un simulador en
 * caso de que alguna de estas tenga un fallo durante su creacion.
 */
void simulador_destruir_en_fallo(simulador_t* simulador) {
	if (!simulador)
		return;

	aux_destruir_pokemon_en_recepcion(simulador->pokemon_en_tratamiento);
	heap_destruir(simulador->recepcion, aux_destruir_pokemon_en_recepcion);
	lista_iterador_destruir(simulador->sala_espera_entrenadores);
	lista_iterador_destruir(simulador->sala_espera_pokemones);
	abb_destruir_todo(simulador->dificultades, aux_destruir_dificultad);
}

simulador_t* simulador_crear(hospital_t* hospital) {
	if (!hospital)
		return NULL;

	simulador_t* simulador = calloc(1, sizeof(simulador_t));
	if (!simulador)
		return NULL;

	bool exito = true;

	simulador->hospital = hospital;
	simulador->pokemon_en_tratamiento = NULL;
	simulador->estadisticas = (EstadisticasSimulacion){
		0,
		.entrenadores_totales = (unsigned)hospital_cantidad_entrenadores(hospital),
		.pokemon_totales = (unsigned)hospital_cantidad_pokemon(hospital),
	};

	simulador->recepcion = heap_crear(aux_comparador_nivel_pokemon);
	if (!(simulador->recepcion))
		exito = false;

	simulador->sala_espera_entrenadores = lista_iterador_crear(hospital->entrenadores);
	if (!(simulador->sala_espera_entrenadores))
		exito = false;

	simulador->sala_espera_pokemones = lista_iterador_crear(hospital->pokemones_orden_llegada);
	if (!(simulador->sala_espera_pokemones))
		exito = false;

	simulador->dificultades = aux_inicializar_dificultades(&(simulador->dificultad_en_uso));
	if (!(simulador->dificultades))
		exito = false;

	simulador->en_curso = true;

	if (!exito) {
		simulador_destruir_en_fallo(simulador);
		return NULL;
	}

	return simulador;
}

/**
 * Recibe un simulador y un puntero valido a las estadisticas a llenar.
 *
 * Simula el evento "ObtenerEstadisticas".
 *
 * En caso de exito retorna ExitoSimulacion y en caso de error ErrorSimulacion.
 */
ResultadoSimulacion simulador_obtener_estadisticas(simulador_t simulador, EstadisticasSimulacion* estadisticas) {
	if (!estadisticas)
		return ErrorSimulacion;

	*estadisticas = (EstadisticasSimulacion){
		.entrenadores_atendidos = simulador.estadisticas.entrenadores_atendidos,
		.entrenadores_totales = simulador.estadisticas.entrenadores_totales,
		.pokemon_atendidos = simulador.estadisticas.pokemon_atendidos,
		.pokemon_en_espera = simulador.estadisticas.pokemon_en_espera,
		.pokemon_totales = simulador.estadisticas.pokemon_totales,
		.puntos = simulador.estadisticas.puntos,
		.cantidad_eventos_simulados = simulador.estadisticas.cantidad_eventos_simulados,
	};

	return ExitoSimulacion;
}

/**
 * Recibe un puntero a simulador valido.
 *
 * Simula el evento "AtenderProximoEntrenador" cargando todos los pokemones de
 * un entrenador en el simulador. Además actualiza la cantidad de pokemones en
 * la sala de espera / recepcion cada vez que se atiende un nuevo entrenador y
 * actualiza el pokemon en tratamiento de ser necesario (en el caso en que no
 * haya pokemones agregados en la recepcion).
 * 
 * En caso de exito retorna ExitoSimulacion y en caso de error ErrorSimulacion.
 */
ResultadoSimulacion simulador_atender_proximo_entrenador(simulador_t* simulador) {
	if (!simulador)
		return ErrorSimulacion;

	entrenador_t* proximo_entrenador = lista_iterador_elemento_actual(simulador->sala_espera_entrenadores);
	if (!proximo_entrenador)
		return ErrorSimulacion;

	bool recepcion_exitosa = aux_agregar_pokemones_de_entrenador_a_recepcion(proximo_entrenador, simulador->sala_espera_pokemones, simulador->recepcion);
	if (!recepcion_exitosa)
		return ErrorSimulacion;

	if (!(simulador->pokemon_en_tratamiento))
		simulador_atender_pokemon_de_menor_nivel(simulador);

	simulador->estadisticas.pokemon_en_espera = (unsigned)heap_tamanio(simulador->recepcion);

	simulador->estadisticas.entrenadores_atendidos++;
	lista_iterador_avanzar(simulador->sala_espera_entrenadores);

	return ExitoSimulacion;
}

/**
 * Recibe un simulador y un puntero valido a la informacion a llenar.
 *
 * Simula el evento "ObtenerInformacionPokemonEnTratamiento" llenando el
 * struct pasada por puntero con los datos actualizados del estado de la
 * simulación.
 *
 * En caso de exito retorna ExitoSimulacion y en caso de error ErrorSimulacion.
 */
ResultadoSimulacion simulador_obtener_informacion_pokemon_en_tratamiento(simulador_t simulador, InformacionPokemon* informacion) {
	if (!informacion)
		return ErrorSimulacion;

	PokemonEnRecepcion* pokemon_en_tratamiento = simulador.pokemon_en_tratamiento;
	char* nombre_pokemon = pokemon_en_tratamiento ? pokemon_en_tratamiento->nombre_pokemon : NULL;
	char* nombre_entrenador = pokemon_en_tratamiento ? pokemon_en_tratamiento->nombre_entrenador : NULL;

	*informacion = (InformacionPokemon){
		.nombre_pokemon = nombre_pokemon,
		.nombre_entrenador = nombre_entrenador,
	};

	if (!pokemon_en_tratamiento)
		return ErrorSimulacion;

	return ExitoSimulacion;
}

/**
 * Recibe un simulador y un puntero valido a del intento a probar.
 *
 * Simula el evento "AdivinarNivelPokemon" comprobando los datos ingresados
 * en el intento actual para ver si se ha adivinado el nivel del pokemon en
 * tratamiento.
 *
 * En caso de exito retorna ExitoSimulacion y en caso de error ErrorSimulacion.
 */
ResultadoSimulacion simulador_adivinar_nivel_pokemon(simulador_t* simulador, Intento* intento) {
	if (!simulador || !intento)
		return ErrorSimulacion;

	PokemonEnRecepcion* pokemon_en_tratamiento = simulador->pokemon_en_tratamiento;
	if (!pokemon_en_tratamiento) {
		intento->es_correcto = false;
		return ErrorSimulacion;
	}

	unsigned (*calcular_puntaje)(unsigned cantidad_intentos) = simulador->dificultad_en_uso.calcular_puntaje;
	int (*verificar_nivel)(unsigned, unsigned) = simulador->dificultad_en_uso.verificar_nivel;

	int resultado = verificar_nivel(intento->nivel_adivinado, (unsigned)pokemon_en_tratamiento->nivel);
	intento->es_correcto = resultado == RESULTADO_CORRECTO;

	if (intento->es_correcto) {
		simulador_atender_pokemon_de_menor_nivel(simulador);
		simulador->estadisticas.pokemon_en_espera = (unsigned)heap_tamanio(simulador->recepcion);

		simulador->estadisticas.puntos += calcular_puntaje(simulador->intentos_actuales);
		simulador->intentos_actuales = 0;
		simulador->estadisticas.pokemon_atendidos++;
	} else {
		simulador->intentos_actuales++;
	}

	intento->resultado_string = simulador->dificultad_en_uso.verificacion_a_string(resultado);

	return ExitoSimulacion;
}

/**
 * Recibe un simulador valido y un puntero con los datos de la nueva
 * dificultad.
 *
 * Simula el evento "AgregarDificultad" tomando los datos de la dificultad que
 * se desea agregar, comprobando su validez, y si los datos resultan ser
 * válidos, se agrega la dificultad a la lista de dificultades existentes en
 * el simulador.
 *
 * En caso de exito retorna ExitoSimulacion y en caso de error ErrorSimulacion.
 */
ResultadoSimulacion simulador_agregar_dificultad(simulador_t* simulador, DatosDificultad* datos_dificultad) {
	if (!simulador || !datos_dificultad)
		return ErrorSimulacion;

	DatosDificultadConId* dificultad =
		aux_crear_dificultad(simulador->dificultades, (int)abb_tamanio(simulador->dificultades), *datos_dificultad);

	if (!dificultad)
		return ErrorSimulacion;

	abb_insertar(simulador->dificultades, dificultad);

	return ExitoSimulacion;
}

/*
 * Recibe un simulador valido y un puntero con los datos de la dificultad
 * a la que se desea cambiar.
 *
 * Simula el evento "SeleccionarDificultad" tomando los datos de la dificultad
 * que se desea seleccionar y cambiando la dificultad actual a la dificultad
 * indicada.
 *
 * En caso de exito retorna ExitoSimulacion y en caso de error ErrorSimulacion.
 */
ResultadoSimulacion simulador_seleccionar_dificultad(simulador_t* simulador, int* id_dificultad) {
	if (!id_dificultad)
		return ErrorSimulacion;

	DatosDificultadConId datos_dificultad_buscada = {
		.id = *id_dificultad};

	DatosDificultadConId* dificultad_encontrada = abb_buscar(simulador->dificultades, &datos_dificultad_buscada);
	if (!dificultad_encontrada)
		return ErrorSimulacion;

	simulador->dificultad_en_uso = *dificultad_encontrada;

	return ExitoSimulacion;
}

/*
 * Recibe un simulador valido y un puntero con los datos de la dificultad
 * buscada.
 *
 * Simula el evento "ObtenerInformacionDificultad" tomando los datos los datos
 * basicos de la dificultad buscada y completando los datos faltantes en caso
 * de que esta dificultad haya sido encontrada. En caso de no haber encontrado
 * la dificultad buscada, se llenan los datos con valores de error
 * preestablecidos.
 *
 * En caso de exito retorna ExitoSimulacion y en caso de error ErrorSimulacion.
 */
ResultadoSimulacion simulador_obtener_informacion_dificultad(simulador_t* simulador, InformacionDificultad* dificultad_buscada) {
	if (!simulador || !dificultad_buscada)
		return ErrorSimulacion;

	DatosDificultadConId datos_dificultad_buscada = {
		.id = dificultad_buscada->id};

	DatosDificultadConId* dificultad_encontrada = abb_buscar(simulador->dificultades, &datos_dificultad_buscada);
	if (!dificultad_encontrada) {
		dificultad_buscada->id = -1;
		dificultad_buscada->nombre_dificultad = NULL;
		dificultad_buscada->en_uso = false;
		return ErrorSimulacion;
	}

	dificultad_buscada->nombre_dificultad = dificultad_encontrada->nombre;
	dificultad_buscada->en_uso = aux_dificultad_esta_en_uso(simulador->dificultad_en_uso, *dificultad_encontrada);

	return ExitoSimulacion;
}

ResultadoSimulacion simulador_simular_evento(simulador_t* simulador, EventoSimulacion evento, void* datos) {
	ResultadoSimulacion res = ErrorSimulacion;

	if (!simulador || !(simulador->en_curso))
		return res;

	simulador->estadisticas.cantidad_eventos_simulados++;

	switch (evento) {
		case ObtenerEstadisticas:
			res = simulador_obtener_estadisticas(*simulador, datos);
			break;
		case AtenderProximoEntrenador:
			res = simulador_atender_proximo_entrenador(simulador);
			break;
		case ObtenerInformacionPokemonEnTratamiento:
			res = simulador_obtener_informacion_pokemon_en_tratamiento(*simulador, datos);
			break;
		case AdivinarNivelPokemon:
			res = simulador_adivinar_nivel_pokemon(simulador, datos);
			break;
		case AgregarDificultad:
			res = simulador_agregar_dificultad(simulador, datos);
			break;
		case SeleccionarDificultad:
			res = simulador_seleccionar_dificultad(simulador, datos);
			break;
		case ObtenerInformacionDificultad:
			res = simulador_obtener_informacion_dificultad(simulador, datos);
			break;
		case FinalizarSimulacion:
			simulador->en_curso = false;
			res = ExitoSimulacion;
			break;
		default:
			simulador->estadisticas.cantidad_eventos_simulados--;
	}

	return res;
}

void simulador_destruir(simulador_t* simulador) {
	if (!simulador)
		return;

	aux_destruir_pokemon_en_recepcion(simulador->pokemon_en_tratamiento);
	heap_destruir(simulador->recepcion, aux_destruir_pokemon_en_recepcion);
	lista_iterador_destruir(simulador->sala_espera_entrenadores);
	lista_iterador_destruir(simulador->sala_espera_pokemones);
	abb_destruir_todo(simulador->dificultades, aux_destruir_dificultad);
	hospital_destruir(simulador->hospital);

	free(simulador);
}
