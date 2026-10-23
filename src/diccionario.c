#include "diccionario.h"
#include <stdio.h>
#include <string.h>

#define CAPACIDAD_INICIAL_MINIMA 3
#define HASH_INICIAL 5381
#define MULTIPLICADOR_DJB2 33
#define CARACTER_NULO '\0'
#define SON_IGUALES 0
#define FACTOR_DE_CARGA_MAXIMO 0.75f

typedef struct celda {
	void *valor;
	char *clave;
	bool fue_borrado;
} celda_t;

struct diccionario {
	celda_t *tabla;
	size_t cantidad;
	size_t capacidad;
};

size_t buscar_indice(diccionario_t* d, char* clave, bool* encontrada){
    size_t indice = funcion_hash(clave) % d->capacidad;
    size_t i = 0;
    bool espacio_vacio = false;
    while (!(*encontrada) && (!espacio_vacio) && (i < d->capacidad)){
        if ((!d->tabla[indice].fue_borrado) && (!d->tabla[indice].clave)){
            espacio_vacio = true;
        }
        else if (son_claves_iguales(d->tabla[indice].clave, clave)){
            *encontrada = true;
        }
        else {
            indice = (indice + 1) % d->capacidad;
            i++;
        }
    }
    return indice;
}

size_t funcion_hash(char *clave)
{
	size_t hash = HASH_INICIAL;
	while (*clave != CARACTER_NULO) {
		hash = hash * MULTIPLICADOR_DJB2 + (size_t)(*clave);
		clave++;
	}
	return hash;
}

bool son_claves_iguales(char *clave1, char *clave2)
{
    if ((!clave1) || (!clave2)) {
        return false;
    }
	return (strcmp(clave1, clave2) == SON_IGUALES);
}

diccionario_t *diccionario_crear(unsigned capacidad_inicial)
{
	if (capacidad_inicial <= CAPACIDAD_INICIAL_MINIMA) {
		capacidad_inicial = CAPACIDAD_INICIAL_MINIMA;
	}
	diccionario_t *diccionario = calloc(1, sizeof(diccionario_t));
	if (!diccionario) {
		return NULL;
	}
	diccionario->capacidad = capacidad_inicial;
	diccionario->tabla = calloc(capacidad_inicial, sizeof(celda_t));
	if (!diccionario->tabla) {
		free(diccionario);
		return NULL;
	}
	return diccionario;
}

/*
 * Inserta clave/valor, devuelve true si pudo o false si no pudo. Si ya existía
 * la clave apunta anterior al valor asociado anteriormente.
 *
 * Si se necesita agrandar la tabla no está permitido hacerla crecer a mas del doble del tamaño.
 *
 *  No es válido insertar claves NULL.
 */
bool diccionario_insertar(diccionario_t *d, char *clave, void *valor,
			  void **anterior)
{
	if ((!d) || (!clave) || (!clave)) {
		return false;
	}
	if ((float)d->cantidad / (float)d->capacidad >=
	    FACTOR_DE_CARGA_MAXIMO) { //Casteo a int para que la divison me de con coma y no redondeado a 0
		return true;
	}
    return false;
}

bool diccionario_buscar(diccionario_t *d, char *clave)
{
	if ((!d) || (!clave)) {
		return false;
	}
	size_t indice = funcion_hash(clave) % d->capacidad;
	size_t i = 0;
	bool encontrada = false;
	bool espacio_vacio = false;
	while ((!encontrada) && (!espacio_vacio) && (i < d->capacidad)) {
		if ((!d->tabla[indice].clave) &&
		    (!d->tabla[indice].fue_borrado)) {
			espacio_vacio = true;
		} else if ((son_claves_iguales(d->tabla[indice].clave,
					       clave))) { //ACA BORRE ALGO
			encontrada = true;
		} else {
			indice = (indice + 1) % d->capacidad;
			i++;
		}
	}
	return encontrada;
}

/*
 * Busca una clave en el diccionario y devuelve el valor asociado.
 */
void *diccionario_obtener(diccionario_t *d, char *clave)
{
	if ((!d) || (!clave)) {
		return NULL;
	}
	void *valor = NULL;
	size_t indice = funcion_hash(clave) % d->capacidad;
	size_t i = 0;
	bool encontrada = false;
	bool espacio_vacio = false;
	while ((!encontrada) && (!espacio_vacio) && (i < d->capacidad)) {
		if ((!d->tabla[indice].clave) &&
		    (!d->tabla[indice].fue_borrado)) {
			espacio_vacio = true;
		} else if ((son_claves_iguales(d->tabla[indice].clave,
					       clave))) {
			encontrada = true;
			valor = d->tabla[indice].valor;
		} else {
			indice = (indice + 1) % d->capacidad;
			i++;
		}
	}
	return valor;
}

/*
 * Elimina una clave de la tabla y devuelve el valor asociado a esa clave o NULL si no existía.
 */
void *diccionario_eliminar(diccionario_t *d, char *clave)
{
	if ((!d) || (!clave)) {
		return NULL;
	}
	void *valor = NULL;
	size_t indice = funcion_hash(clave) % d->capacidad;
	size_t i = 0;
	bool encontrada = false;
	bool espacio_vacio = false;
	while ((!encontrada) && (!espacio_vacio) && (i < d->capacidad)) {
		if ((!d->tabla[indice].clave) &&
		    (!d->tabla[indice].fue_borrado)) {
			espacio_vacio = true;
		} else if ((son_claves_iguales(d->tabla[indice].clave,
					       clave))) {
			encontrada = true;
			valor = d->tabla[indice].valor;
			d->tabla[indice].valor = NULL;
			free(d->tabla[indice].clave);
			d->tabla[indice].clave = NULL;
			d->tabla[indice].fue_borrado = true;
			d->cantidad--;
		} else {
			indice = (indice + 1) % d->capacidad;
			i++;
		}
	}
	return valor;
}

size_t diccionario_cantidad(diccionario_t *d)
{
	if (!d) {
		return 0;
	}
	return (d->cantidad);
}

size_t diccionario_iterar(diccionario_t *d, bool (*f)(char *, void *, void *),
			  void *extra)
{
    if ((!d) || (!f)) {
    return 0;
    }
	bool seguir_iterando = true;
	size_t i = 0;
	size_t invocaciones = 0;
	while ((seguir_iterando) && (i < d->capacidad)) {
		if (d->tabla[i].clave) {
			seguir_iterando =
				f(d->tabla[i].clave, d->tabla[i].valor, extra);
			invocaciones++;
		}
		i++;
	}
	return invocaciones;
}

void diccionario_destruir(diccionario_t *d)
{
	diccionario_destruir_todo(d, NULL);
}

void diccionario_destruir_todo(diccionario_t *d, void (*destructor)(void *))
{
	if (!d) {
		return;
	}
	for (size_t i = 0; i < d->capacidad; i++) {
		if (d->tabla[i].clave) {
			if (destructor) {
				destructor(d->tabla[i].valor);
			}
			free(d->tabla[i].clave);
		}
	}
	free(d->tabla);
	free(d);
}