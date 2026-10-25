#include "diccionario.h"
#include <stdio.h>
#include <string.h>

#define CAPACIDAD_INICIAL_MINIMA 3
#define HASH_INICIAL 5381
#define MULTIPLICADOR_DJB2 33
#define CARACTER_NULO '\0'
#define SON_IGUALES 0
#define FACTOR_DE_CARGA_MAXIMO 0.75f
#define MULTIPLICADOR_CAPACIDAD 2
#define PASO_LINEAL 1

typedef struct par {
	void *valor;
	char *clave;
	bool eliminado;
} par_t;

struct diccionario {
	par_t *tabla;
	size_t cantidad;
	size_t capacidad;
};

/*
 * La clave recibida por parámetro es válida.
 * 
 * Devuelve el valor numérico (hash) generado.
 */
size_t funcion_hash(char *clave)
{
	size_t hash = HASH_INICIAL;
	while ((*clave) != CARACTER_NULO) {
		hash = (hash * MULTIPLICADOR_DJB2) + (size_t)(*clave);
		clave++;
	}
	return hash;
}

/*
 * Las claves recibidas por parámetro son válidas.
 * 
 * Devuelve true si ambas claves son iguales, false en caso contrario.
 */
bool claves_coinciden(char *clave1, char *clave2)
{
	return (strcmp(clave1, clave2) == SON_IGUALES);
}

/*
 * El diccionario y la clave existen y son válidos. El puntero 'encontrada' es válido.	
 * Actualiza el booleano 'encontrada' si halla la clave.
 * Devuelve el índice donde se encuentra la clave, o en su defecto, el índice 
 * del primer espacio libre o eliminado disponible para insertarla.
 */
size_t buscar_indice(diccionario_t *d, char *clave, bool *encontrada)
{
	size_t indice = funcion_hash(clave) % d->capacidad;
	size_t i = 0;
	bool espacio_vacio = false;
	while (!(*encontrada) && (!espacio_vacio) && (i < d->capacidad)) {
		if ((!d->tabla[indice].eliminado) &&
		    (!d->tabla[indice].clave)) {
			espacio_vacio = true;
		} else if ((d->tabla[indice].clave) &&
			   (claves_coinciden(d->tabla[indice].clave, clave))) {
			*encontrada = true;
		} else {
			indice = (indice + PASO_LINEAL) % d->capacidad;
			i++;
		}
	}
	return indice;
}

/*
 * La clave recibida es un puntero válido.
 * Devuelve un puntero a la nueva cadena copiada, o NULL si falla la reserva.
 */
char *copiar_clave(char *clave)
{
	size_t longitud = strlen(clave);
	char *copia = calloc(longitud + 1, sizeof(char));
	if (!copia) {
		return NULL;
	}
	strcpy(copia, clave);
	return copia;
}

/*
 * El diccionario y la clave son válidos. El índice está dentro del rango de la tabla.
 * Duplica la clave en el heap.
 * Devuelve true si la inserción fue exitosa, o false si falló la reserva de memoria para la clave.
 */
bool insertar_par(diccionario_t *d, size_t indice, char *clave, void *valor,
		  void **anterior)
{
	char *clave_copiada = copiar_clave(clave);
	if (!clave_copiada) {
		return false;
	}
	if (anterior) {
		*anterior = NULL;
	}
	d->tabla[indice].clave = clave_copiada;
	d->tabla[indice].valor = valor;
	d->tabla[indice].eliminado = false;
	d->cantidad++;
	return true;
}

/*
 * El arreglo 'nueva_tabla' existe y su capacidad es mayor a cero. 
 * La clave es válida.
 * Busca la primera posición libre en el nuevo arreglo utilizando prueba lineal y 
 * traslada los punteros correspondientes
 */
void reubicar_par(par_t *nueva_tabla, size_t nueva_capacidad, char *clave,
		  void *valor)
{
	size_t indice = funcion_hash(clave) % nueva_capacidad;
	while (nueva_tabla[indice].clave) {
		indice = (indice + PASO_LINEAL) % nueva_capacidad;
	}
	nueva_tabla[indice].clave = clave;
	nueva_tabla[indice].valor = valor;
	nueva_tabla[indice].eliminado = false;
}

/*
 * El diccionario existe y es válido.
 * Devuelve true si la redimensión fue exitosa, o false si falló la reserva de memoria.
 */
bool rehashear(diccionario_t *d)
{
	size_t nueva_capacidad = d->capacidad * MULTIPLICADOR_CAPACIDAD;
	par_t *nueva_tabla = calloc(nueva_capacidad, sizeof(par_t));
	if (!nueva_tabla) {
		return false;
	}
	for (size_t i = 0; i < d->capacidad; i++) {
		if (d->tabla[i].clave) {
			reubicar_par(nueva_tabla, nueva_capacidad,
				     d->tabla[i].clave, d->tabla[i].valor);
		}
	}
	free(d->tabla);
	d->tabla = nueva_tabla;
	d->capacidad = nueva_capacidad;
	return true;
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
	diccionario->capacidad = (size_t)capacidad_inicial;
	diccionario->tabla = calloc(capacidad_inicial, sizeof(par_t));
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
	if ((!d) || (!clave)) {
		return false;
	}
	float factor_carga = (float)d->cantidad / (float)d->capacidad;
	if (factor_carga >= FACTOR_DE_CARGA_MAXIMO) {
		if (!rehashear(d)) {
			return false;
		}
	}
	bool clave_encontrada = false;
	size_t indice = buscar_indice(d, clave, &clave_encontrada);
	if (clave_encontrada) {
		if (anterior) {
			*anterior = d->tabla[indice].valor;
		}
		d->tabla[indice].valor = valor;
		return true;
	}
	return (insertar_par(d, indice, clave, valor, anterior));
}

bool diccionario_buscar(diccionario_t *d, char *clave)
{
	if ((!d) || (!clave)) {
		return false;
	}
	bool clave_encontrada = false;
	buscar_indice(d, clave, &clave_encontrada);
	return clave_encontrada;
}

void *diccionario_obtener(diccionario_t *d, char *clave)
{
	if ((!d) || (!clave)) {
		return NULL;
	}
	bool clave_encontrada = false;
	size_t indice = buscar_indice(d, clave, &clave_encontrada);
	if (clave_encontrada) {
		return (d->tabla[indice].valor);
	}
	return NULL;
}

void *diccionario_eliminar(diccionario_t *d, char *clave)
{
	if ((!d) || (!clave)) {
		return NULL;
	}
	bool clave_encontrada = false;
	size_t indice = buscar_indice(d, clave, &clave_encontrada);
	if (clave_encontrada) {
		free(d->tabla[indice].clave);
		d->tabla[indice].clave = NULL;
		d->tabla[indice].eliminado = true;
		d->cantidad--;
		return (d->tabla[indice].valor);
	}
	return NULL;
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