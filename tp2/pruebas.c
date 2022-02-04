#include <stdbool.h>

#include "pa2mm.h"
#include "src/heap.h"
#include "src/hospital.h"
#include "src/juego.h"
#include "src/simulador.h"
#include "string.h"

#define PUNTAJE_INICIAL 100

bool ignorar_pokemon(pokemon_t* p) {
	p = p;
	return true;
}

/* No intenten esto en sus casas */
/* Ya vamos a ver como evitar esto en el TDA Lista */
struct {
	pokemon_t* pokemon[500];
	size_t cantidad;
} acumulados;

void resetear_acumulados() {
	acumulados.cantidad = 0;
}

bool acumular_pokemon(pokemon_t* p) {
	acumulados.pokemon[acumulados.cantidad] = p;
	acumulados.cantidad++;
	return true;
}

bool acumular_pokemon_hasta_miltank(pokemon_t* p) {
	acumulados.pokemon[acumulados.cantidad] = p;
	acumulados.cantidad++;
	return strcmp(pokemon_nombre(p), "miltank");
}

bool acumulados_en_orden_correcto() {
	if (acumulados.cantidad < 2)
		return true;
	pokemon_t* anterior = acumulados.pokemon[0];
	for (int i = 1; i < acumulados.cantidad; i++) {
		pokemon_t* actual = acumulados.pokemon[i];
		if (strcmp(pokemon_nombre(anterior), pokemon_nombre(actual)) > 0)
			return false;
	}
	return true;
}

/* Pruebas: Hospital */

void puedoCrearYDestruirUnHospital() {
	hospital_t* h = NULL;

	pa2m_afirmar((h = hospital_crear()), "Crear un hospital devuelve un hospital");

	pa2m_afirmar(hospital_cantidad_entrenadores(h) == 0, "Un hospital se crea con cero entrenadores");
	pa2m_afirmar(hospital_cantidad_pokemon(h) == 0, "Un hospital se crea con cero pokemon");

	pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon) == 0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

	hospital_destruir(h);
}

void dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema() {
	hospital_t* h = NULL;

	pa2m_afirmar(hospital_cantidad_entrenadores(h) == 0, "Un hospital NULL tiene cero entrenadores");
	pa2m_afirmar(hospital_cantidad_pokemon(h) == 0, "Un hospital NULL tiene cero pokemon");

	pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon) == 0, "Recorrer los pokemon de un hospital NULL resulta en 0 pokemon recorridos");

	hospital_destruir(h);
}

void dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital() {
	hospital_t* h = hospital_crear();

	pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/archivo_vacio.hospital"), "Puedo leer un archivo vacío");

	pa2m_afirmar(hospital_cantidad_entrenadores(h) == 0, "Un hospital vacío tiene cero entrenadores");
	pa2m_afirmar(hospital_cantidad_pokemon(h) == 0, "Un hospital vacío tiene tiene cero pokemon");

	pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon) == 0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

	hospital_destruir(h);
}

void dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital() {
	hospital_t* h = hospital_crear();

	pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/un_entrenador.hospital"), "Puedo leer un archivo con un entrenador");

	pa2m_afirmar(hospital_cantidad_entrenadores(h) == 1, "El hospital tiene 1 entrenador");
	pa2m_afirmar(hospital_cantidad_pokemon(h) == 3, "El hospital tiene 3 pokemon");

	resetear_acumulados();
	pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon) == 3, "Recorrer los pokemon resulta en 3 pokemon recorridos");
	pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

	hospital_destruir(h);
}

void dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital() {
	hospital_t* h = hospital_crear();

	pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");

	pa2m_afirmar(hospital_cantidad_entrenadores(h) == 5, "El hospital tiene 5 entrenadores");
	pa2m_afirmar(hospital_cantidad_pokemon(h) == 24, "El hospital tiene 24 pokemon");

	resetear_acumulados();
	pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon) == 24, "Recorrer los pokemon resulta en 24 pokemon recorridos");
	pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

	hospital_destruir(h);
}

void dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital() {
	hospital_t* h = hospital_crear();

	pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");
	pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer otro archivo con varios entrenadores");
	pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un tercer archivo con varios entrenadores");

	pa2m_afirmar(hospital_cantidad_entrenadores(h) == 15, "El hospital tiene 15 entrenadores");
	pa2m_afirmar(hospital_cantidad_pokemon(h) == 72, "El hospital tiene 72 pokemon");

	resetear_acumulados();
	pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon) == 72, "Recorrer los pokemon resulta en 72 pokemon recorridos");
	pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

	hospital_destruir(h);
}

/* Pruebas simulador */

void dadoUnHospital_alCrearUnSimulador_seRetornaElSimulador() {
	simulador_t* simulador = simulador_crear(NULL);

	pa2m_afirmar(simulador == NULL, "Crear un simulador con un hospital NULL retorna NULL");

	hospital_t* hospital = hospital_crear();
	simulador = simulador_crear(hospital);

	pa2m_afirmar(simulador != NULL, "Crear un simulador con un hospital valido retorna un simulador valido");

	simulador_destruir(simulador);
}

void dadoUnSimuladorNULL_alSimularUnEvento_seRetornarError() {
	simulador_t* simulador = NULL;

	ResultadoSimulacion res = simulador_simular_evento(simulador, ObtenerEstadisticas, NULL);
	pa2m_afirmar(res == ErrorSimulacion, "Simular un evento con un simulador NULL resulta en Error");

	simulador_destruir(simulador);
}

void dadoUnSimulador_alSimularUnEventoInvalido_seRetornaError() {
	hospital_t* hospital = hospital_crear();
	hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

	simulador_t* simulador = simulador_crear(hospital);

	ResultadoSimulacion res = simulador_simular_evento(simulador, 69, NULL);
	pa2m_afirmar(res == ErrorSimulacion, "Al simular un evento invalido se retorna Error");

	// Si fueran eventos validos la cantidad de eventos simulados deberia aumentar +3
	res = simulador_simular_evento(simulador, 69, NULL);
	res = simulador_simular_evento(simulador, 69, NULL);
	res = simulador_simular_evento(simulador, 69, NULL);

	EstadisticasSimulacion estadisticas;
	res = simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);

	pa2m_afirmar(estadisticas.cantidad_eventos_simulados == 1, "Al simular un evento invalido no incrementa la cantidad de eventos simulados");

	simulador_destruir(simulador);
}

void dadoUnHospitalVacio_alObtenerLasEstadisticas_seObtienenLasEstadisticasEsperadas() {
	hospital_t* hospital = hospital_crear();
	simulador_t* simulador = simulador_crear(hospital);

	EstadisticasSimulacion estadisticas;
	ResultadoSimulacion res = simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);

	pa2m_afirmar((res == ExitoSimulacion) && (estadisticas.cantidad_eventos_simulados == 1),
			"Obtener las estadisticas del simulador correctamente retorna Exito y la cantidad de eventos simulados se incrementa");
	pa2m_afirmar((estadisticas.entrenadores_totales == 0) && (estadisticas.pokemon_totales == 0),
			"Al obtener las estadisticas de un hospital vacio, los entrenadores y pokemones totales valen 0");

	simulador_destruir(simulador);
}

void dadoUnHospital_alObtenerLasEstadisticas_seObtienenLasEstadisticasEsperadas() {
	hospital_t* hospital = hospital_crear();
	hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

	simulador_t* simulador = simulador_crear(hospital);

	ResultadoSimulacion res = simulador_simular_evento(simulador, ObtenerEstadisticas, NULL);

	pa2m_afirmar(res == ErrorSimulacion, "Obtener las estadisticas del simulador pasando un dato NULL retorna Error");

	EstadisticasSimulacion estadisticas;
	res = simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);

	pa2m_afirmar((estadisticas.entrenadores_totales == hospital_cantidad_entrenadores(hospital)) &&
			(estadisticas.pokemon_totales == hospital_cantidad_pokemon(hospital)),
			"Las cantidades de entrenadores y pokemones de las estadisticas coinciden con las cantidades del hospital");

	simulador_destruir(simulador);
}

void dadoUnSimulador_alFinalizarLaSimulacion_yaNoSePuedenSeguirSimulandoEventos() {
	hospital_t* hospital = hospital_crear();
	simulador_t* simulador = simulador_crear(hospital);

	ResultadoSimulacion res = simulador_simular_evento(simulador, FinalizarSimulacion, NULL);

	pa2m_afirmar(res == ExitoSimulacion, "Finalizar la simulacion de un simulador activo retorna Exito");

	res = simulador_simular_evento(simulador, FinalizarSimulacion, NULL);
	pa2m_afirmar(res == ErrorSimulacion, "Finalizar la simulacion de un simulador previamente finalizado retorna Error");

	res = simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
	pa2m_afirmar(res == ErrorSimulacion, "Una vez se finaliza un simulador, al intentar simular mas eventos se retorna Error");

	simulador_destruir(simulador);
}

void dadoUnHospitalVacio_alAtenderAlProximoEntrenador_seRetornaError() {
	hospital_t* hospital = hospital_crear();
	simulador_t* simulador = simulador_crear(hospital);

	ResultadoSimulacion res = simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

	pa2m_afirmar(res == ErrorSimulacion, "Atender al proximo entrenador en un hospital vacio retorna Error");

	simulador_destruir(simulador);
}

void dadoUnHospital_alAtenderAlProximoEntrenador_seAtiendeCorrectamente() {
	hospital_t* hospital = hospital_crear();
	hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

	simulador_t* simulador = simulador_crear(hospital);

	ResultadoSimulacion res = simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

	pa2m_afirmar(res == ExitoSimulacion, "Al atender al proximo entrenador en un hospital se retorna Exito");

	simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

	EstadisticasSimulacion estadisticas;
	simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);

	pa2m_afirmar(estadisticas.entrenadores_atendidos == 2, "Al atender a un entrenador la cantidad total de entrenadores atendidos en el simulador aumenta en 1");

	simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
	simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
	simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

	simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);

	pa2m_afirmar(estadisticas.pokemon_en_espera == hospital_cantidad_pokemon(hospital) - 1,
			"Luego de agregar todos los pokemones del hospital y no haber atendido a ninguno, la cantidad de pokemones en la recepcion es igual al total de pokemones menos el pokemon en el consultorio")

	res = simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

	pa2m_afirmar(res == ErrorSimulacion, "Intentar atender al proximo entrenador cuando ya se han atendido todos los entrenadores retorna Error")

	simulador_destruir(simulador);
}

void dadoUnHospitalVacio_alObtenerLaInformacionDelPokemonEnTratamiento_seObtieneLaInformacionEsperada() {
	hospital_t* hospital = hospital_crear();
	simulador_t* simulador = simulador_crear(hospital);

	ResultadoSimulacion res = simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, NULL);
	pa2m_afirmar(res == ErrorSimulacion, "Al obtener los datos del pokemon en tratamiento pasando un dato NULL, se retorna ErrorSimulacion");

	InformacionPokemon informacion = {NULL};
	res = simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &informacion);

	pa2m_afirmar((informacion.nombre_pokemon == NULL) && (informacion.nombre_entrenador == NULL),
			"Los datos del pokemon en tratamiento si no hay ningun pokemon en tratamiento son NULL");

	simulador_destruir(simulador);
}

void dadoUnHospital_alAtenderAlProximoEntrenador_losPokemonesSeAtiendenConLaPrioridadCorrecta() {
	hospital_t* hospital = hospital_crear();
	hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

	simulador_t* simulador = simulador_crear(hospital);

	simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

	InformacionPokemon informacion = {};
	ResultadoSimulacion res = simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &informacion);

	pa2m_afirmar(res == ExitoSimulacion, "Atender al proximo entrenador cuando hay entrenadores disponibles retorna Exito");
	pa2m_afirmar(strcmp(informacion.nombre_pokemon, "rampardos") == 0,
			"Luego de atender al primer entrenador, el pokemon en tratamiento pasa a ser \"rampardos\", que es el que tiene el menor nivel");

	simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

	res = simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &informacion);

	pa2m_afirmar(strcmp(informacion.nombre_pokemon, "rampardos") == 0,
			"Luego de atender a varios entrenadores sin haber terminado de atender al pokemon en tratamiento, este ultimo se mentiene en tratamiento");

	Intento intento = {
		.nivel_adivinado = 10,
	};

	simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
	simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &informacion);

	pa2m_afirmar(strcmp(informacion.nombre_pokemon, "toxicroak") == 0, "Luego de atender \"rampardos\" el pokemon que pasa a ser atentido es \"toxicroak\"");

	intento.nivel_adivinado = 20;

	simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
	simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &informacion);

	pa2m_afirmar(strcmp(informacion.nombre_pokemon, "charizard") == 0, "Luego de atender \"toxicroak\" el pokemon que pasa a ser atentido es \"charizard\"");

	simulador_destruir(simulador);
}

void dadoUnSimuladorSinPokemonEnTratamiento_alIntentarAdivinarElNivelDelPokemonEnTratamiento_seRetornaError() {
	hospital_t* hospital = hospital_crear();
	simulador_t* simulador = simulador_crear(hospital);

	ResultadoSimulacion res = simulador_simular_evento(simulador, AdivinarNivelPokemon, NULL);

	pa2m_afirmar(res == ErrorSimulacion, "Intentar adivinar el nivel del pokemon en tratamiento con un dato NULL retorna Error");

	InformacionPokemon informacion;
	res = simulador_simular_evento(simulador, AdivinarNivelPokemon, &informacion);

	pa2m_afirmar(res == ErrorSimulacion, "Intentar adivinar el nivel del pokemon en un hospital sin pokemones retorna Error");

	simulador_destruir(simulador);
}

void dadoUnSimulador_alIntentarAdivinarElNivelDelPokemonEnTratamiento_seAdivinaCorrectamente() {
	hospital_t* hospital = hospital_crear();
	hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

	simulador_t* simulador = simulador_crear(hospital);

	simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

	Intento intento;
	intento.nivel_adivinado = 0;

	ResultadoSimulacion res = simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);

	pa2m_afirmar(res == ExitoSimulacion, "Al intentar adivinar el nivel de un pokemon correctamente se retorna Exito");
	pa2m_afirmar(intento.es_correcto == false, "Adivinar un invel incorrecto resulta en un intento fallido");
	pa2m_afirmar(strcmp(intento.resultado_string, "Te quedaste corto por entre 10 y 25 niveles") == 0,
			"Al fallar un intento se obtiene el mensaje de resultado esperado");

	intento.nivel_adivinado = 10;
	res = simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);

	pa2m_afirmar(intento.es_correcto == true, "Intentar adivinar el nivel del pokemon correctamente resulta en un intento exitoso");
	pa2m_afirmar(strcmp(intento.resultado_string, "Adivinaste Crack") == 0,
			"Al acertar un intento se obtiene el mensaje de resultado esperado");

	EstadisticasSimulacion estadisticas;
	simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);

	pa2m_afirmar(estadisticas.pokemon_atendidos == 1, "Al adivinar el nivel de un pokemon, la cantidad de atendidos aumenta en 1");

	simulador_destruir(simulador);
}

void dadoUnSimulador_alIntentarAdivinarElNivelDeVariosPokemones_seAdivinanCorrectamente() {
	hospital_t* hospital = hospital_crear();
	hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

	simulador_t* simulador = simulador_crear(hospital);

	simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

	Intento intento;
	intento.nivel_adivinado = 10;
	ResultadoSimulacion res = simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);

	intento.nivel_adivinado = 20;
	simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);

	intento.nivel_adivinado = 43;
	simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);

	intento.nivel_adivinado = 85;
	simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);

	intento.nivel_adivinado = 69;
	res = simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);
	res = simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento);

	pa2m_afirmar(res == ErrorSimulacion, "Intentar adivinar el nivel de un pokemon cuando ya se han atendido todos retorna Error");
	pa2m_afirmar(intento.es_correcto == false, "Intentar adivinar el nivel de un pokemon cuando ya se han atendido todos resulta en un intento fallido");

	EstadisticasSimulacion estadisticas;
	simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);

	pa2m_afirmar(estadisticas.pokemon_atendidos == 4, "Al adivinar el nivel de todos los pokemones posibles, la cantidad de atendidos es el total de pokemones que habian en la recepcion");
	pa2m_afirmar(estadisticas.pokemon_en_espera == 0, "Al haber atendido a todos los pokemones la cantidad de pokemenes en espera es 0");

	simulador_destruir(simulador);
}

void dadoUnSimulador_alIntentarAdivinarElNivelConDiferentesDificultades_laDificultadelJuegoSeAjustaCorrectamente() {
	hospital_t* hospital = hospital_crear();
	hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores");

	simulador_t* simulador = simulador_crear(hospital);

	ResultadoSimulacion res = simulador_simular_evento(simulador, SeleccionarDificultad, NULL);
	pa2m_afirmar(res == ErrorSimulacion, "Al seleccionar una dificultad pasando un dato NULL resulta en Error");

	int id_nueva_dificultad = 69;
	res = simulador_simular_evento(simulador, SeleccionarDificultad, &id_nueva_dificultad);

	pa2m_afirmar(res == ErrorSimulacion, "Seleccionar un dificultad con un ID inexistente resulta en Error");

	id_nueva_dificultad = 1;
	res = simulador_simular_evento(simulador, SeleccionarDificultad, &id_nueva_dificultad);

	pa2m_afirmar(res == ExitoSimulacion, "Seleccionar una dificultad nueva resulta en Exito");

	InformacionDificultad informacion;
	informacion.id = 1;
	simulador_simular_evento(simulador, ObtenerInformacionDificultad, &informacion);

	pa2m_afirmar(informacion.en_uso == true, "Luego de seleccionar la dificultad con ID 1, esta dificultad pasa a estar en uso");

	informacion.id = 0;
	simulador_simular_evento(simulador, ObtenerInformacionDificultad, &informacion);

	pa2m_afirmar(informacion.en_uso == false, "Luego de cambiar la dificultad, la dificultad que estaba en uso antes ahora ya no esta en uso");

	simulador_destruir(simulador);
}

void dadoUnSimulador_alCrearElSimulador_esteTiene3DificultadesIniciales() {
	hospital_t* hospital = hospital_crear();
	hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

	simulador_t* simulador = simulador_crear(hospital);

	ResultadoSimulacion res = simulador_simular_evento(simulador, ObtenerInformacionDificultad, NULL);

	pa2m_afirmar(res == ErrorSimulacion, "Obtener la informacion de una dificultad pasando un dato NULL resulta en Error");

	InformacionDificultad informacion;
	informacion.id = 0;

	res = simulador_simular_evento(simulador, ObtenerInformacionDificultad, &informacion);

	pa2m_afirmar(res == ExitoSimulacion, "Obtener la informacion de una de las dificultades creadas retorna Exito");
	pa2m_afirmar(strcmp(informacion.nombre_dificultad, "Facil") == 0, "La dificultad con ID 0 tiene el nombre \"Facil\"");
	pa2m_afirmar(informacion.en_uso == true, "La dificultad \"Facil\" esta en uso por defecto");

	informacion.id = 1;
	simulador_simular_evento(simulador, ObtenerInformacionDificultad, &informacion);
	pa2m_afirmar(strcmp(informacion.nombre_dificultad, "Media") == 0, "La dificultad con ID 1 tiene el nombre \"Media\"");

	informacion.id = 2;
	simulador_simular_evento(simulador, ObtenerInformacionDificultad, &informacion);
	pa2m_afirmar(strcmp(informacion.nombre_dificultad, "Dificil") == 0, "La dificultad con ID 2 tiene el nombre \"Dificil\"");

	informacion.id = 69;
	res = simulador_simular_evento(simulador, ObtenerInformacionDificultad, &informacion);

	pa2m_afirmar(res == ErrorSimulacion, "Buscar una dificultad con un ID que no existe retorna Error");
	pa2m_afirmar(informacion.nombre_dificultad == NULL, "Al buscar una dificultad con un ID que no existe, el nombre de la dificultad buscada en el dato pasado pasa a ser NULL");
	pa2m_afirmar(informacion.id == -1, "Al buscar una dificultad con un ID que no existe, el ID buscado en el dato pasado pasa a ser -1");

	simulador_destruir(simulador);
}

unsigned calcular_puntaje_dificultad_nueva(unsigned cantidad_intentos) {
	return PUNTAJE_INICIAL / cantidad_intentos + 1;
}

int verificar_nivel_dificultad_nueva(unsigned nivel_adivinado, unsigned nivel_pokemon) {
	return (int)nivel_pokemon - (int)nivel_adivinado;
}

const char* verificacion_a_string_dificultad_nueva(int resultado_verificacion) {
	if (resultado_verificacion == 0)
		return "Adivinaste Crack";

	return "No Adivinaste Crack";
}

void dadaUnaDificultad_alAgregarlaAlSimulador_laDificultadSeAgregaCorrectamente() {
	hospital_t* hospital = hospital_crear();
	hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

	simulador_t* simulador = simulador_crear(hospital);

	ResultadoSimulacion res = simulador_simular_evento(simulador, AgregarDificultad, NULL);

	pa2m_afirmar(res == ErrorSimulacion, "Intentar agregar una dificultad con un dato NULL resulta en Error");

	DatosDificultad dificultad_invalida = {
		.nombre = NULL,
		.calcular_puntaje = NULL,
		.verificar_nivel = NULL,
		.verificacion_a_string = NULL,
	};

	res = simulador_simular_evento(simulador, AgregarDificultad, &dificultad_invalida);

	pa2m_afirmar(res == ErrorSimulacion, "Intentar agregar un dificultad con datos invalidos (como nombre NULL), resulta en Error");

	DatosDificultad dificultad_con_nombre_repetido = {
		.nombre = "Facil",
		.calcular_puntaje = calcular_puntaje_dificultad_nueva,
		.verificar_nivel = verificar_nivel_dificultad_nueva,
		.verificacion_a_string = verificacion_a_string_dificultad_nueva,
	};

	res = simulador_simular_evento(simulador, AgregarDificultad, &dificultad_con_nombre_repetido);

	pa2m_afirmar(res == ErrorSimulacion, "Al intentar agregar una dificultad con nombre repetido se retorna Error");

	DatosDificultad nueva_dificultad = {
		.nombre = "Nueva Dificultad",
		.calcular_puntaje = calcular_puntaje_dificultad_nueva,
		.verificar_nivel = verificar_nivel_dificultad_nueva,
		.verificacion_a_string = verificacion_a_string_dificultad_nueva,
	};

	res = simulador_simular_evento(simulador, AgregarDificultad, &nueva_dificultad);

	pa2m_afirmar(res == ExitoSimulacion, "Agregar una nueva dificultad retorna Exito");

	InformacionDificultad informacion;
	informacion.id = 3;

	simulador_simular_evento(simulador, ObtenerInformacionDificultad, &informacion);

	pa2m_afirmar(strcmp(informacion.nombre_dificultad, nueva_dificultad.nombre) == 0,
			"Se encuentra el nombre de la nueva dificultad agregada");

	simulador_destruir(simulador);
}

/* Pruebas heap */

int comparador_heap_pruebas(void* _a, void* _b) {
	if (!_a || !_b)
		return 0;

	int* a = _a;
	int* b = _b;

	if (*a > *b)
		return 1;
	else if (*a < *b)
		return -1;

	return 0;
}

void dadoUnComparador_alCrearUnHeap_seCreaCorrectamente() {
	heap_t* heap = heap_crear(NULL);

	pa2m_afirmar(heap == NULL, "Al crear un heap con un comparador NULL, el comparador es igual a NULL");

	heap = heap_crear(comparador_heap_pruebas);
	pa2m_afirmar(heap != NULL, "Al crear un heap con un comparador valido, se retorna un heap");
	pa2m_afirmar(heap_tamanio(heap) == 0, "Al crear un heap el tamanio inicial es igual a 0");

	heap_destruir(heap, NULL);
}

void dadoUnHeap_alInsertarElementos_seInsertanCorrectamente() {
	heap_t* heap = NULL;

	int e0 = 3019, e1 = 5828, e2 = 1257;

	size_t res;
	res = heap_insertar(heap, &e0);

	pa2m_afirmar(res == 0, "Al insertar en un heap NULL, se retorna la cantidad de elementos que es igual a 0");

	heap = heap_crear(comparador_heap_pruebas);

	res = heap_insertar(heap, &e0);
	res = heap_insertar(heap, &e1);
	res = heap_insertar(heap, &e2);

	pa2m_afirmar(res == heap_tamanio(heap), "Al insertar elementos en un heap se retorna el tamanio correcto del heap");
	pa2m_afirmar(res == 3, "Al insertar 3 elementos en un heap, se retorna la cantidad de elementos que es igual a 3");

	heap_destruir(heap, NULL);
}

void dadoUnHeapMinimal_alQuitarElementos_seRetornaElMenorElementoDelHeap() {
	heap_t* heap = NULL;

	pa2m_afirmar(heap_extraer_raiz(heap) == NULL, "Al extraer la raiz de un heap NULL, se retorna NULL");

	heap = heap_crear(comparador_heap_pruebas);
	pa2m_afirmar(heap_extraer_raiz(heap) == NULL, "Al extraer la raiz de un heap vacio, se retorna NULL");

	int e0 = 3019, e1 = 5828, e2 = 1257, e3 = 4761, e4 = 2779, e5 = 629,
		e6 = 9689, e7 = 2728, e8 = 2783, e9 = 2774;

	heap_insertar(heap, &e0);
	heap_insertar(heap, &e1);
	heap_insertar(heap, &e2);
	heap_insertar(heap, &e3);
	heap_insertar(heap, &e4);
	heap_insertar(heap, &e5);
	heap_insertar(heap, &e6);
	heap_insertar(heap, &e7);
	heap_insertar(heap, &e8);
	heap_insertar(heap, &e9);

	int extraido = *(int*)heap_extraer_raiz(heap);

	pa2m_afirmar(extraido == e5, "Al extraer la raiz de un heap minimal, se obtiene el elemento de menor valor entre todos los insertados");
	pa2m_afirmar(heap_tamanio(heap) == 9, "Al extraer la raiz de un heap su tamanio se reduce");

	extraido = *(int*)heap_extraer_raiz(heap);
	pa2m_afirmar(extraido == e2, "Al extraer la raiz de un heap minimal por segunda vez, se obtiene el segundo elemento de menor valor entre todos los insertados");

	heap_extraer_raiz(heap);
	heap_extraer_raiz(heap);
	heap_extraer_raiz(heap);
	heap_extraer_raiz(heap);
	heap_extraer_raiz(heap);
	heap_extraer_raiz(heap);
	heap_extraer_raiz(heap);
	heap_extraer_raiz(heap);

	pa2m_afirmar(heap_tamanio(heap) == 0, "Al extraer todos los elementos de un heap, el tamanio es igual a 0");

	heap_destruir(heap, NULL);
}

int main() {
	// Pruebas TP1
	/* pa2m_nuevo_grupo("Pruebas de creación y destrucción");
	puedoCrearYDestruirUnHospital();

	pa2m_nuevo_grupo("Pruebas con NULL");
	dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema();

	pa2m_nuevo_grupo("Pruebas con un archivo vacío");
	dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital();

	pa2m_nuevo_grupo("Pruebas con un archivo de un entrenador");
	dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital();

	pa2m_nuevo_grupo("Pruebas con un archivo de varios entrenadores");
	dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital();

	pa2m_nuevo_grupo("Pruebas con mas de un archivo");
	dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital(); */

	// Pruebas TP2
	pa2m_nuevo_grupo("Pruebas simulador");
	dadoUnHospital_alCrearUnSimulador_seRetornaElSimulador();
	dadoUnSimuladorNULL_alSimularUnEvento_seRetornarError();
	dadoUnSimulador_alSimularUnEventoInvalido_seRetornaError();
	dadoUnHospitalVacio_alObtenerLasEstadisticas_seObtienenLasEstadisticasEsperadas();
	dadoUnHospital_alObtenerLasEstadisticas_seObtienenLasEstadisticasEsperadas();
	dadoUnSimulador_alFinalizarLaSimulacion_yaNoSePuedenSeguirSimulandoEventos();

	pa2m_nuevo_grupo("Pruebas atender entrenadores");
	dadoUnHospitalVacio_alAtenderAlProximoEntrenador_seRetornaError();
	dadoUnHospital_alAtenderAlProximoEntrenador_seAtiendeCorrectamente();
	dadoUnHospitalVacio_alObtenerLaInformacionDelPokemonEnTratamiento_seObtieneLaInformacionEsperada();
	dadoUnHospital_alAtenderAlProximoEntrenador_losPokemonesSeAtiendenConLaPrioridadCorrecta();

	pa2m_nuevo_grupo("Pruebas adivinar nivel pokemon");
	dadoUnSimuladorSinPokemonEnTratamiento_alIntentarAdivinarElNivelDelPokemonEnTratamiento_seRetornaError();
	dadoUnSimulador_alIntentarAdivinarElNivelDelPokemonEnTratamiento_seAdivinaCorrectamente();
	dadoUnSimulador_alIntentarAdivinarElNivelDeVariosPokemones_seAdivinanCorrectamente();

	pa2m_nuevo_grupo("Pruebas dificultades");
	dadoUnSimulador_alIntentarAdivinarElNivelConDiferentesDificultades_laDificultadelJuegoSeAjustaCorrectamente();
	dadoUnSimulador_alCrearElSimulador_esteTiene3DificultadesIniciales();
	dadaUnaDificultad_alAgregarlaAlSimulador_laDificultadSeAgregaCorrectamente();

	pa2m_nuevo_grupo("Pruebas heap");
	dadoUnComparador_alCrearUnHeap_seCreaCorrectamente();
	dadoUnHeap_alInsertarElementos_seInsertanCorrectamente();
	dadoUnHeapMinimal_alQuitarElementos_seRetornaElMenorElementoDelHeap();

	return pa2m_mostrar_reporte();
}
