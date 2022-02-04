#include "split.h"
#include <stdlib.h>

size_t contar_substrings(const char* string, char separador) {
	if (*string == 0) 
		return 1;
	return (*string == separador ? 1 : 0) + contar_substrings(string + 1, separador);
}

size_t contar_longitud_substring(const char* string, char separador) {
	if (*string == separador || *string == '\n')
		return 0;
	return 1 + contar_longitud_substring(string + 1, separador);
}

char* duplicar_string(const char* string, size_t longitud_string) {
	char* substring = malloc((longitud_string + 1) * sizeof(char));
	if (!substring)
		return NULL;

	for (size_t i = 0; i < longitud_string; i++)
		substring[i] = string[i];

	substring[longitud_string] = 0;
	return substring;
}

void liberar_vector_strings(char** vector_strings) {
	while (*vector_strings) {
		free(*vector_strings);
		vector_strings++;
	}
	free(vector_strings);
}

char** split(const char* string, char separador){
	if (!string)
		return NULL;

	size_t cantidad_substrings = contar_substrings(string, separador);

	char** vector_strings = malloc((cantidad_substrings + 1) * sizeof(char*));
	if (!vector_strings)
		return NULL;

	for (size_t i = 0; i < cantidad_substrings; i++) {
		size_t longitud_substring = contar_longitud_substring(string, separador);
		char* substring = duplicar_string(string, longitud_substring);
		if (!substring) {
			liberar_vector_strings(vector_strings);
			return NULL;
		}

		vector_strings[i] = substring;
		string += longitud_substring + 1;
	}

	vector_strings[cantidad_substrings] = NULL;

	return vector_strings;
}
