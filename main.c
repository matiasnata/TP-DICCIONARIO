#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "src/diccionario.h"
#include "src/leer_linea.h"

#define ARGUMENTOS_ESPERADOS 3
#define ARGV_POSICION_ARCHIVO 1
#define ARGV_POSICION_FRASE 2
#define CAPACIDAD_INICIAL 50
#define DELIMITADOR_CSV ','
#define DELIMITADOR_ESPACIO ' '
#define CARACTER_NULO '\0'
#define MODO_LECTURA "r"
#define EXITO 0
#define ERROR -1

/*
 * La cadena recibida por parámetro es un puntero válido.
 * 
 * Devuelve un puntero a la nueva cadena duplicada, o NULL si falla la reserva.
 */
char *duplicar_cadena(char *cadena)
{
	size_t longitud = strlen(cadena);
	char *copia = calloc(longitud + 1, sizeof(char));
	if (!copia) {
		return NULL;
	}
	strcpy(copia, cadena);
	return copia;
}

/*
 * La cadena recibida es válida.
 * Busca la primera aparición del delimitador en la cadena. Si lo encuentra,
 * lo reemplaza por el carácter nulo para dividir el string en dos partes.
 * 
 * Devuelve un puntero al inicio de la segunda parte, o NULL si no encontró el delimitador.
 */
char *separar_por_delimitador(char *cadena, char delimitador)
{
	size_t i = 0;
	while ((cadena[i] != CARACTER_NULO) && (cadena[i] != delimitador)) {
		i++;
	}
	if (cadena[i] == delimitador) {
		cadena[i] = CARACTER_NULO;
		return (&cadena[i + 1]);
	}
	return NULL;
}

/*
 * El diccionario y la línea son punteros válidos.
 * Separa la línea en clave y valor, duplica el valor en memoria dinámica y lo
 * inserta en el diccionario. Si la clave ya existía, libera el valor anterior.
 * 
 * Devuelve true si pudo procesar e insertar correctamente, o false en caso de error.
 */
bool procesar_linea_csv(diccionario_t *diccionario, char *linea)
{
	char *valor = separar_por_delimitador(linea, DELIMITADOR_CSV);
	if (!valor) {
		return false;
	}
	char *valor_copia = duplicar_cadena(valor);
	if (!valor_copia) {
		return false;
	}
	void *valor_anterior = NULL;
	if (!diccionario_insertar(diccionario, linea, valor_copia,
				  &valor_anterior)) {
		free(valor_copia);
		return false;
	}
	if (valor_anterior) {
		free(valor_anterior);
	}
	return true;
}

/*
 * La ruta del archivo es un puntero válido.
 * Abre el archivo CSV, crea el diccionario y carga todos los pares clave-valor
 * leyendo línea por línea. Cierra el archivo al finalizar.
 * 
 * Devuelve un puntero al diccionario cargado, o NULL en caso de error.
 */
diccionario_t *crear_diccionario_desde_csv(char *ruta_archivo)
{
	FILE *archivo = fopen(ruta_archivo, MODO_LECTURA);
	if (!archivo) {
		return NULL;
	}
	diccionario_t *diccionario = diccionario_crear(CAPACIDAD_INICIAL);
	if (!diccionario) {
		fclose(archivo);
		return NULL;
	}
	char *linea = leer_linea(archivo);
	while (linea) {
		procesar_linea_csv(diccionario, linea);
		free(linea);
		linea = leer_linea(archivo);
	}
	fclose(archivo);
	return diccionario;
}

/*
 * El diccionario y la palabra son punteros válidos.
 * Busca la palabra en el diccionario. Si existe, imprime su traducción;
 * en caso contrario, imprime la palabra original.
 */
void imprimir_palabra_traducida(diccionario_t *diccionario, char *palabra)
{
	char *traduccion = diccionario_obtener(diccionario, palabra);
	if (traduccion) {
		printf("%s", traduccion);
	} else {
		printf("%s", palabra);
	}
}

/*
 * El diccionario y frase original son punteros válidos.
 * Crea una copia de la frase para no modificar el argumento original, la separa
 * palabra por palabra e imprime su traducción por pantalla.
 */
void traducir_frase(diccionario_t *diccionario, char *frase_original)
{
	char *frase_copia = duplicar_cadena(frase_original);
	if (!frase_copia) {
		return;
	}
	char *palabra_actual = frase_copia;
	while (palabra_actual) {
		char *siguiente_palabra = separar_por_delimitador(
			palabra_actual, DELIMITADOR_ESPACIO);
		imprimir_palabra_traducida(diccionario, palabra_actual);
		if (siguiente_palabra) {
			printf(" ");
		}
		palabra_actual = siguiente_palabra;
	}
	printf("\n");
	free(frase_copia);
}

int main(int argc, char *argv[])
{
	if (argc != ARGUMENTOS_ESPERADOS) {
		printf("Error. La cantidad de argumentos no es la esperada. Recordar que se debe incluir el archivo\n");
		return ERROR;
	}
	diccionario_t *diccionario =
		crear_diccionario_desde_csv(argv[ARGV_POSICION_ARCHIVO]);
	if (!diccionario) {
		printf("Error: No se pudo reservar memoria.\n");
		return ERROR;
	}
	traducir_frase(diccionario, argv[ARGV_POSICION_FRASE]);
	diccionario_destruir_todo(diccionario, free);
	return EXITO;
}