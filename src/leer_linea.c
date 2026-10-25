#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "leer_linea.h"

#define ERROR -1
#define TAMANIO_INICIAL 10
#define MULTIPLICADOR_TAMANIO 2
#define ESPACIO_NULO 1
#define CARACTER_NULO '\0'
#define SALTO_DE_LINEA '\n'

int leer_linea_ptr(char **ptr, size_t *tamaño, FILE *archivo)
{
	if ((!ptr) || (!*ptr) || (!archivo)) {
		return ERROR;
	}
	int caracter_leido = fgetc(archivo);
	if (caracter_leido == EOF) {
		return ERROR;
	}
	size_t tope = 0;
	while ((caracter_leido != EOF) && (caracter_leido != SALTO_DE_LINEA)) {
		if ((tope + ESPACIO_NULO) >= *tamaño) {
			size_t nuevo_tamanio = *tamaño * MULTIPLICADOR_TAMANIO;
			char *ptr_aux = realloc(*ptr, nuevo_tamanio);
			if (!ptr_aux) {
				return ERROR;
			}
			*ptr = ptr_aux;
			*tamaño = nuevo_tamanio;
		}
		(*ptr)[tope] = (char)caracter_leido;
		tope++;
		caracter_leido = fgetc(archivo);
	}
	(*ptr)[tope] = CARACTER_NULO;
	return (int)tope;
}

char *leer_linea(FILE *archivo)
{
	if (!archivo) {
		return NULL;
	}
	size_t tamaño_inicial = TAMANIO_INICIAL;
	char *ptr_string = malloc(TAMANIO_INICIAL * sizeof(char));
	if (!ptr_string) {
		return NULL;
	}
	int resultado = leer_linea_ptr(&ptr_string, &tamaño_inicial, archivo);
	if (resultado == ERROR) {
		free(ptr_string);
		return NULL;
	}
	return ptr_string;
}
