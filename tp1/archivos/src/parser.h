#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

#define CANTIDAD_CARACTERES 1024

/**
  * Lee una linea de un archivo con longitud desconocida.
  * 
  * Recibe un archivo previamente abierto y valido.
  *
  * Devuelve un puntero a caracteres (string) que contiene la informacion
  * obtenida directamente de la linea leida (sin procesar).
  */
char* leer_linea(FILE* archivo);

/**
  * Divide una linea en diferentes campos en base al formato de archivo
  * especificado.
  *
  * En caso de error devuelve NULL. En caso contrario, devuelve un vector
  * de strings, en el que cada string es uno de los campos encontrados en la
  * linea leida.
  */
char** obtener_informacion_linea(char* linea_archivo);

#endif
