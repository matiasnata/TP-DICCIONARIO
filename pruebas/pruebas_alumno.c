#include "pa2m.h"
#include "../src/diccionario.h"
#include <stdio.h>
#include <stdbool.h>

bool iterar_hasta_uno_diccionario(char *clave, void *valor, void *extra)
{
	(void)clave;
	(void)valor;
	int *contador = (int *)extra;
	(*contador)++;
	return ((*contador) < 1);
}

void crear_diccionario_devuelve_puntero_valido()
{
	diccionario_t *dic = diccionario_crear(10);
	pa2m_afirmar(dic != NULL,
		     "Crear un diccionario devuelve un puntero válido");
	diccionario_destruir(dic);
}

void pedir_cantidad_diccionario_nulo_devuelve_cero()
{
	pa2m_afirmar(diccionario_cantidad(NULL) == 0,
		     "Pedir la cantidad de un diccionario nulo devuelve 0");
}

void insertar_elemento_valido_aumenta_cantidad()
{
	diccionario_t *dic = diccionario_crear(10);
	int a = 10;
	pa2m_afirmar(diccionario_insertar(dic, "Clave1", &a, NULL) == true,
		     "Se puede insertar un par clave-valor válido");
	diccionario_destruir(dic);
}

void insertar_en_diccionario_nulo_devuelve_false()
{
	int a = 1;
	pa2m_afirmar(diccionario_insertar(NULL, "Clave1", &a, NULL) == false,
		     "Insertar en un diccionario nulo devuelve false");
}

void insertar_clave_nula_devuelve_false()
{
	diccionario_t *dic = diccionario_crear(10);
	int a = 1;
	pa2m_afirmar(diccionario_insertar(dic, NULL, &a, NULL) == false,
		     "Insertar una clave nula devuelve false");
	diccionario_destruir(dic);
}

void actualizar_clave_existente_devuelve_dato_anterior()
{
	diccionario_t *dic = diccionario_crear(10);
	int a = 10;
	int b = 20;
	void *anterior = NULL;
	diccionario_insertar(dic, "Clave1", &a, NULL);

	diccionario_insertar(dic, "Clave1", &b, &anterior);
	pa2m_afirmar(anterior == &a,
		     "Actualizar una clave asigna el valor viejo a anterior");
	diccionario_destruir(dic);
}

void obtener_clave_valida_devuelve_puntero()
{
	diccionario_t *dic = diccionario_crear(10);
	int a = 10;
	diccionario_insertar(dic, "Clave1", &a, NULL);
	pa2m_afirmar(diccionario_obtener(dic, "Clave1") == &a,
		     "Obtener con clave existente devuelve el dato correcto");
	diccionario_destruir(dic);
}

void obtener_clave_inexistente_devuelve_null()
{
	diccionario_t *dic = diccionario_crear(10);
	int a = 10;
	diccionario_insertar(dic, "Clave1", &a, NULL);
	pa2m_afirmar(diccionario_obtener(dic, "ClaveFalsa") == NULL,
		     "Obtener con clave inexistente devuelve NULL");
	diccionario_destruir(dic);
}

void buscar_clave_valida_devuelve_true()
{
	diccionario_t *dic = diccionario_crear(10);
	int a = 10;
	diccionario_insertar(dic, "Clave1", &a, NULL);
	pa2m_afirmar(diccionario_buscar(dic, "Clave1") == true,
		     "Buscar una clave existente devuelve true");
	diccionario_destruir(dic);
}

void buscar_en_diccionario_nulo_devuelve_false()
{
	pa2m_afirmar(diccionario_buscar(NULL, "Clave1") == false,
		     "Buscar en un diccionario nulo devuelve false");
}

void eliminar_clave_existente_devuelve_dato()
{
	diccionario_t *dic = diccionario_crear(10);
	int a = 10;
	diccionario_insertar(dic, "Clave1", &a, NULL);
	pa2m_afirmar(diccionario_eliminar(dic, "Clave1") == &a,
		     "Eliminar una clave existente devuelve su puntero");
	diccionario_destruir(dic);
}

void eliminar_clave_inexistente_devuelve_null()
{
	diccionario_t *dic = diccionario_crear(10);
	int a = 10;
	diccionario_insertar(dic, "Clave1", &a, NULL);
	pa2m_afirmar(diccionario_eliminar(dic, "ClaveFalsa") == NULL,
		     "Intentar eliminar una clave inexistente devuelve NULL");
	diccionario_destruir(dic);
}

void eliminar_en_diccionario_nulo_devuelve_null()
{
	pa2m_afirmar(diccionario_eliminar(NULL, "Clave1") == NULL,
		     "Intentar eliminar en diccionario nulo devuelve NULL");
}

void iterador_interno_corta_segun_funcion()
{
	diccionario_t *dic = diccionario_crear(10);
	int a = 1;
	int b = 2;
	int contador = 0;
	diccionario_insertar(dic, "Clave1", &a, NULL);
	diccionario_insertar(dic, "Clave2", &b, NULL);

	size_t iteradas = diccionario_iterar(dic, iterar_hasta_uno_diccionario,
					     &contador);
	pa2m_afirmar(
		iteradas == 1,
		"El iterador interno corta correctamente cuando la función devuelve false");
	diccionario_destruir(dic);
}

void iterador_interno_diccionario_nulo_devuelve_cero()
{
	int contador = 0;
	pa2m_afirmar(diccionario_iterar(NULL, iterar_hasta_uno_diccionario,
					&contador) == 0,
		     "Iterar un diccionario nulo devuelve 0 iteraciones");
}

int main()
{
	pa2m_nuevo_grupo(" PRUEBAS DE DICCIONARIO: CREACION Y CANTIDAD ");
	crear_diccionario_devuelve_puntero_valido();
	pedir_cantidad_diccionario_nulo_devuelve_cero();
	pa2m_nuevo_grupo(" PRUEBAS DE DICCIONARIO: INSERCION ");
	insertar_elemento_valido_aumenta_cantidad();
	insertar_en_diccionario_nulo_devuelve_false();
	insertar_clave_nula_devuelve_false();
	actualizar_clave_existente_devuelve_dato_anterior();
	pa2m_nuevo_grupo(" PRUEBAS DE DICCIONARIO: OBTENCION Y BUSQUEDA ");
	obtener_clave_valida_devuelve_puntero();
	obtener_clave_inexistente_devuelve_null();
	buscar_clave_valida_devuelve_true();
	buscar_en_diccionario_nulo_devuelve_false();
	pa2m_nuevo_grupo(" PRUEBAS DE DICCIONARIO: ELIMINACION ");
	eliminar_clave_existente_devuelve_dato();
	eliminar_clave_inexistente_devuelve_null();
	eliminar_en_diccionario_nulo_devuelve_null();
	pa2m_nuevo_grupo(" PRUEBAS DE DICCIONARIO: ITERADORES ");
	iterador_interno_corta_segun_funcion();
	iterador_interno_diccionario_nulo_devuelve_cero();
	return pa2m_mostrar_reporte();
}