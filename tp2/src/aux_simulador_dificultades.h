#ifndef AUX_SIMULADOR_DIFICULTADES_H__
#define AUX_SIMULADOR_DIFICULTADES_H__

#include <stddef.h>

#include "abb.h"
#include "simulador.h"

typedef struct {
    int id;
    const char* nombre;
    unsigned (*calcular_puntaje)(unsigned cantidad_intentos);
    int (*verificar_nivel)(unsigned nivel_adivinado, unsigned nivel_pokemon);
    const char* (*verificacion_a_string)(int resultado_verificacion);
} DatosDificultadConId;

/**
 * Se utiliza para crear nuevas dificultades dado los datos corretos. Se encarga
 * de guardar las dificultades con el id especificado. Devuelve la nueva dificultad
 * en caso de exito para que esta sea agregada a las dificultades ya existentes o 
 * NULL en caso de error.
 */
DatosDificultadConId* aux_crear_dificultad(abb_t* dificultades, int id, DatosDificultad datos_dificultad);

/**
 * Genera las dificultades iniciales que existen en el simulador: Facil, Media y
 * Dificil, cada una con sus respectivos datos y ids.
 * 
 * En caso de exito retorna las nuevas dificultades y en caso de errror NULL.
 */
abb_t* aux_inicializar_dificultades(DatosDificultadConId* dificultad_por_defecto);

/*
 * Evalua si la dificultad recibida esta actualmente en uso o no.
 */
bool aux_dificultad_esta_en_uso(DatosDificultadConId dificultad_en_uso, DatosDificultadConId dificultad_buscada);

/*
 * Libera de memoria las dificultades almacenadas una vez se dejan de utilizar.
 */
void aux_destruir_dificultad(void* _dificultad);

#endif
