#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "split.h"

char*
leer_linea(FILE* archivo)
{
	if (!archivo)
		return NULL;

	size_t caracteres_leidos = 0;
	size_t max_caracteres = CANTIDAD_CARACTERES;
	char* linea = malloc(max_caracteres * sizeof(char));
	if (!linea)
		return NULL;

	while (fgets(linea + caracteres_leidos, (int)max_caracteres - (int)caracteres_leidos, archivo)) {
		size_t longitud_string = strlen(linea + caracteres_leidos);
		if (longitud_string > 0 && *(linea + caracteres_leidos + longitud_string - 1) == '\n') {
			*(linea + caracteres_leidos + longitud_string - 1) = 0;
			return linea;
		} else {
			char* linea_aux = realloc(linea, max_caracteres * 2);
			if (!linea_aux) {
				free(linea);
				return NULL;
			}

			linea = linea_aux;
			max_caracteres *= 2;
		}
		caracteres_leidos += longitud_string;
	}

	if (!caracteres_leidos) {
		free(linea);
		return NULL;
	}

	return linea;
}

char**
obtener_informacion_linea(char* linea_archivo)
{
	char** informacion_linea = split(linea_archivo, ';');
	if (!informacion_linea)
		return NULL;

	return informacion_linea;
}

