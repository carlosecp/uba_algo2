#include "split.h"

#include <stdlib.h>

#include "parser_hospital.h"

size_t contar_substrings(const char* string, char separador) {
    if (*string == 0)
        return 1;

    if (*string == separador)
        return 1 + contar_substrings(string + 1, separador);

    return contar_substrings(string + 1, separador);
}

size_t contar_longitud_substring(const char* string, char separador) {
    size_t i = 0;

    while (string[i] != 0 && string[i] != separador) i++;

    return i;
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

char** split(const char* string, char separador) {
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
            free_vector_strings(vector_strings);
            free(vector_strings);
            return NULL;
        }

        vector_strings[i] = substring;
        string += longitud_substring + 1;
    }

    vector_strings[cantidad_substrings] = NULL;

    return vector_strings;
}

void free_vector_strings(char** vector_strings) {
    while (*vector_strings) {
        free(*vector_strings);
        vector_strings++;
    }
}
