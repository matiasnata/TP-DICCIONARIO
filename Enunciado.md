# TDA Diccionario

Se pide implementar un diccionario basado en una **Tabla de Hash cerrada** (direccionamiento abierto) en C99. Para ello se brindan las firmas de las funciones públicas a implementar y **se deja a criterio del alumno la creación de las estructuras y funciones privadas del TDA** para el correcto funcionamiento de la **Tabla de Hash** cumpliendo con las buenas prácticas de programación. La función de **Hash** a utilizar será interna de la implementación y también debe ser decidida por el alumno. Para esta implementación las claves admitidas por la tabla serán solamente strings (no nulos). Adicionalmente se pide la creación de un iterador interno que sea capaz de recorrer las claves almacenadas en la tabla. También se pide que la implementación haga una copia de cada clave a almacenar.

En el archivo **main.c** se debe implementar un programa que por línea de comandos reciba el nombre de un archivo y un texto y utilice la información del archivo para hacer reemplazos por palabras en el texto provisto por el usuario. Por ejemplo para el siguiente archivo:

```csv
hola,chau
algoritmos,logaritmos
dos,sapo
```

Se comporte de la siguiente manera:

  - **./main archivo.csv "hola mundo"**: muestra por pantalla **"chau mundo"**
  - **./main archivo.csv "algoritmos dos"**: muestra por pantalla **"logaritmos sapo"**

El TDA entregado deberá compilar y pasar las pruebas dispuestas por la cátedra sin errores, adicionalmente estas pruebas deberán ser ejecutadas **sin pérdida de memoria**. Por último (y como siempre), se deben entregar las pruebas elaboradas por el alumno que verifiquen el correcto funcionamiento de la implementación realizada.

## Parte teórica

Explica teoricamente los siguientes puntos (no necesariamente en orden, pero por favor usando diagramas):

   - Qué es un diccionario
    - Explica 3 formas diferentes de implementar un diccionario (tabla de hash cuenta como 1 sola implementación posible)
   - Qué es una función de hash y qué características debe tener para nuestro problema en particular
   - Qué es una tabla de Hash y los diferentes métodos de resolución de colisiones vistos (encadenamiento, probing, etc)
   - Por qué es importante el tamaño de la tabla (tanto para tablas abiertas como cerradas)
     - Dado que en una tabla abierta se pueden encadenar colisiones sin importar el tamaño de la tabla, ¿Realmente importa el tamaño?
   - Mas te vale que expliques con dibujos/diagramas
   - No hace falta aclarar, pero se debe hacer un análisis de complejidad de las operaciones.
