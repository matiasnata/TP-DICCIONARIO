## Información del estudiante

* Matias Armida
* 114941
* armidamatias@gmail.com
* matiasnata

## Índice
* [1. Instrucciones](#1-Instrucciones)
  * [1.1. Compilar el proyecto](#11-Compilar-el-proyecto)
  * [1.2. Ejecutar las pruebas](#12-Ejecutar-las-pruebas)
  * [1.3. Ejecutar el programa con Valgrind](#13-Ejecutar-el-programa-con-Valgrind)
* [2. Funcionamiento](#2-Funcionamiento)
* [3. Estructura](#3-Estructura)
  * [3.1. Diagrama de memoria](#31-Diagrama-de-memoria)
  * [3.2. Análisis de complejidades](#32-Análisis-de-complejidades)
* [4. Decisiones de diseño y/o complejidades de implementación](#4-Decisiones-de-diseño-yo-complejidades-de-implementación)
* [5. Respuestas a las preguntas teóricas](#5-Respuestas-a-las-preguntas-teóricas)

## 1. Instrucciones

### 1.1. Compilar el proyecto
```bash
make main
```

### 1.2. Ejecutar las pruebas
```bash
make pruebas
```

### 1.3. Ejecutar el programa con Valgrind
```bash
make valgrind
```

## 2. Funcionamiento

El TDA Diccionario implementado funciona como una colección de pares clave-valor, donde cada elemento se identifica de manera única a través de una cadena de texto (clave). El objetivo del TDA es la optimización de las operaciones básicas como el almacenamiento, la búsqueda, la actualización y la eliminación de estos datos con una alta eficiencia temporal, apuntando a un acceso casi instantáneo O(1).

Al iniciar el programa, la clave de texto ingresada es procesada por una función matemática interna de hash (el algoritmo DJB2 en este caso). Esta función transforma la cadena de caracteres en un valor numérico, el cual se utiliza como índice para ubicar y guardar el par (clave, valor) dentro de un arreglo dinámico (tabla de hash).

Durante la ejecución, a medida que aumenta la cantidad de pares, es probable que dos claves de texto distintas al aplicarles la función hash tengan como resultado el mismo índice numérico, provocando una colisión. Para resolver esto (la colisión), el diccionario utiliza un direccionamiento abierto con prueba lineal (linear probing). Si la posición original calculada ya se encuentra ocupada por otra clave, el programa avanza secuencialmente por las celdas contiguas del arreglo hasta encontrar un espacio libre donde insertar el nuevo par.

Para garantizar el funcionamiento correcto del diccionario, el mismo cuenta con dos características primordiales:

1) Eliminación de pares (Lápidas): Cuando se elimina un elemento de la tabla, se indica que en esa posición anteriormente había un par y fue eliminado. Esto es crucial para indicar que la celda está disponible para nuevas inserciones, pero al mismo tiempo advierte a los procesos de búsqueda que no deben detenerse ahí, ya que podrían existir elementos más adelante que colisionaron en el pasado.

2) Redimensión dinámica (Rehash): Cuando el arreglo dinámico (tabla) se llena más allá del límite (factor de carga >= 0,75), las probabilidades de colisión aumentan en demasía y el rendimiento decae. Al detectar esto, se redimensiona la capacidad del diccionario y se reubican únicamente los pares que estaban anteriormente. Además de reducir el factor de carga, elimina las lápidas acumuladas para mejorar el rendimiento.

## 3. Estructura
Para implementar el TDA Diccionario, decidí realizar dos estructuras: una estructura administradora principal (diccionario) y una estructura interna para cada par del mismo.

1) El Diccionario (diccionario_t):
Para implementar la estructura, decidí hacerlo con tres campos: un puntero al arreglo dinámico que contiene las celdas donde se almacenan los pares, un contador con la cantidad de elementos actualmente insertados, y un campo que almacena la capacidad total del arreglo. Ambos campos son size_t ya que un número negativo no tendría sentido.
Mantener un registro constante de la cantidad y la capacidad es indispensable, ya que permite calcular el factor de carga en tiempo real y decidir exactamente cuándo es necesario realizar la redimensión (rehash) para evitar que el rendimiento decaiga y que las colisiones aumenten.

2) La Celda / Par (celda_t):
Es la estructura que representa cada posición individual dentro del arreglo principal. Cada una cuenta con un puntero a la cadena de texto (clave), un puntero genérico al dato almacenado (valor), y un campo booleano (fue_borrado). Es necesario incluir este último campo (que actúa como "lápida") es necesario para el funcionamiento del diccionario cerrado: permite diferenciar un casillero que siempre estuvo vacío de uno que almacenó un elemento que luego fue eliminado. Esto asegura que el algoritmo de búsqueda por prueba lineal no se interrumpa erróneamente al encontrarse con un espacio que fue liberado, permitiendo encontrar claves que sufrieron colisiones en el pasado.

//////

### 3.1. Diagrama de memoria

Para facilitar la explicación y 

### 3.2. Análisis de complejidades

1) Diccionario_crear: tiene una complejidad de O(1) ya que reserva memoria dinámica para una única estructura diccionario_t inicializando sus campos en 0 (calloc), asignando el tamaño inicial del array y devolviendo la misma en caso de éxito. Es una operación de tiempo constante.

2) Diccionario_cantidad: tiene complejidad O(1) ya que tiene como parámetro un puntero al TDA diccionario y devuelve la cantidad de elementos del mismo. Accede de forma directa a un campo de la estructura (d->cantidad) mediante una comprobación condicional (if (!d)), lo cual no depende de la cantidad de elementos almacenados, resultando en tiempo constante O(1)

3) Diccionario_buscar: tiene una complejidad de O(1) en casos óptimos y de O(n) en el peor caso. Tiene como parámetros el diccionario y la clave a buscar. En el caso óptimo, la función de hash calcula el índice y encuentra el elemento inmediatamente. Sin embargo, en el peor caso, si todas las claves colisionan, el algoritmo se ve obligado a avanzar linealmente por el arreglo comparando hasta iterar sobre los n elementos insertados hasta el momento.

4) Diccionario_obtener: tiene una complejidad de O(1) en casos óptimos y de O(n) en el peor caso. Tiene como parámetros el diccionario y la clave, y utiliza exactamente la misma lógica de cálculo de índice y prueba lineal que la búsqueda. En el caso óptimo accede al valor directo, mientras que en el peor escenario de colisiones, deberá iterar sobre las n posiciones para ubicar la celda y retornar el valor asociado a la clave.

5) Diccionario_eliminar: tiene una complejidad de O(1) en casos óptimos y de O(n) en el peor caso ya que tiene como parámetros el diccionario y la clave, y utiliza exactamente la misma lógica de cálculo de índice y prueba lineal que la búsqueda. En el caso óptimo encuentra la celda objetivo inmediatamente para aplicar la eliminación, mientras que en un escenario de colisiones se ve obligada a iterar sobre las n posiciones del arreglo antes de poder ubicar el elemento y marcarlo como borrado.

6) Diccionario_destruir: tiene una complejidad de O(n) ya que tiene como parámetro el diccionario y se encarga de recorrer secuencialmente cada una de las posiciones del arreglo dinámico. Sin importar cuántos elementos válidos haya, debe iterar linealmente por toda la capacidad de la tabla para verificar qué celdas están ocupadas y liberar la memoria de cada clave copiada individualmente.

7) Diccionario_destruir_todo: tiene una complejidad de O(n) ya que tiene como parámetros el diccionario y una función destructora. Al igual que la función diccionario_destruir, requiere iterar linealmente por todas las celdas del arreglo para liberar las claves y simultáneamente, aplicar la función destructora recibida a cada uno de los valores asociados a los pares.

8) Diccionario_insertar: tiene una complejidad de O(1) en el caso óptimo, y de O(n) en el peor caso. Tiene como parámetros el diccionario, la clave, el valor a almacenar y un puntero doble para guardar un eventual valor reemplazado. En el escenario óptimo, la función de hash calcula un índice que apunta a una celda disponible (o a la misma clave si ya existía), logrando copiar la cadena e insertar el par en tiempo constante. Sin embargo, en el peor caso la complejidad aumenta a O(n) por dos posibles motivos: El índice calculado sea una celda previamente ocupada, que obligue a la prueba lineal a iterar comparando sobre las n posiciones hasta encontrar un espacio. El otro motivo es que la cantidad de elementos alcance el factor de carga máximo (0.75) y se deba ejecutar la función de redimensionamiento (rehashear), la cual debe recorrer toda la capacidad de la tabla vieja para reubicar linealmente los n pares existentes en la nueva tabla.

## 4. Decisiones de diseño y/o complejidades de implementación
Explicar las decisiones de diseño y/o las complejidades de implementación que hubo durante la resolución del TP.

## 4. Decisiones de diseño y/o complejidades de implementación (EJEMPLO)
La mayor complejidad en el TP se encuentra en la función `foo` que requiere hacer...; es por esto que decidí.... Además, decidí que el programa haga... para mejorar la implementación.

## 5. Respuestas a las preguntas teóricas

### 5.1. ¿Qué es un diccionario?

Un diccionario es un Tipo de Dato Abstracto (TDA) que permite almacenar una colección de datos bajo un formato de pares clave-valor. Su característica fundamental es que cada clave actúa como un identificador exclusivo para consultar, actualizar o eliminar su valor correspondiente. Si bien existen variantes conocidas como "multidiccionarios" que permiten claves repetidas, en la implementación clásica y en la que nos centramos para este trabajo, las claves deben ser estrictamente únicas.
El propósito principal de esta estructura no es mantener los elementos en un orden específico, sino organizar la información internamente de una forma inteligente implementando una función hash acorde a la situación, para que el acceso sea lo mas directo posible. Esto permite que las operaciones básicas de búsqueda, inserción y eliminación se resuelvan con una eficiencia cercana a O(1) en el caso promedio, garantizando una respuesta casi instantánea sin importar si hay cien o un millón de datos almacenados.

### 5.2 ¿Explica 3 formas diferentes de implementar un diccionario (tabla de hash cuenta como 1 sola implementación posible)?

1) Tabla de Hash: Es la implementación desarrollada en este trabajo. Utiliza una función matemática para transformar la clave de texto en un índice de un arreglo, permitiendo almacenar y buscar datos con una eficiencia casi instantánea de O(1) en el caso promedio. Su principal limitación es que los pares clave-valor quedan distribuidos de manera aleatoria, por lo que no se puede iterar sobre ellos siguiendo un orden lógico.

2) Vector de Direccionamiento Directo: consiste en un arreglo donde la propia clave actúa directamente como el índice (por ejemplo, el dato con la clave "15" se guarda exactamente en la posición 15 del vector). Esto garantiza operaciones de búsqueda, inserción y eliminación rapidísimas en un O(1), sin colisiones ni funciones matemáticas complejas

3) Lista Enlazada: los pares simplemente se guardan uno detrás de otro a medida que ingresan, logrando una inserción de O(1) si se agregan al final o al inicio. Sin embargo para localizar o eliminar un elemento requiere recorrer la colección entera secuencialmente comparando las claves, lo que resulta en una complejidad muy ineficiente de O(n).

### 5.3 ¿Qué es una función de hash y qué características debe tener para nuestro problema en particular?

Una función de hash es un algoritmo matemático que toma un dato de entrada de longitud variable (en nuestro caso, una cadena de texto) y lo transforma en un valor numérico entero de tamaño fijo. Este número resultante se procesa posteriormente, por lo general, aplicando la operación módulo con la capacidad de la tabla para obtener un número dentro de la capacidad de la misma, el cual representa el índice del arreglo donde se debe guardar o buscar el elemento.

Para nuestro problema particular (un diccionario con direccionamiento abierto, tabla de hash cerrada y probing linear), la función de hash debe cumplir con tres características fundamentales:

1) Determinismo: Ante la misma clave de texto ingresada, la operación de hash debe garantizar el mismo resultado sin importar la cantidad de veces que se ingrese la clava. Si el resultado variara, sería imposible localizar un dato que ya fue insertado en el arreglo.

2) Distribución uniforme: La función de hash debe dispersar los índices generados de la manera más equitativa posible a lo largo de toda la tabla. Al utilizar linear probing para resolver colisiones, una mala distribución provocaría la formación de agrupamientos de celdas ocupadas contiguas. Esto incrementaría exponencialmente las colisiones futuras y destruiría la eficiencia de O(1) o cercana.

### 5.4 ¿Qué es una tabla de Hash y los diferentes métodos de resolución de colisiones vistos (encadenamiento, probing, etc)?

Una tabla de hash es una implementación específica y muy eficiente de un diccionario. Consiste en un arreglo y una función de hash que transforma las claves en índices válidos dentro de ese arreglo. Como la capacidad es finita y el universo de claves es  infinito, es inevitable que dos claves distintas generen el mismo índice numérico. Cuando sucede que dos claves distintas dan como resultado el mismo índice al aplicarles la función hash, se lo llama colisión y se resuelve mediante diferentes métodos:

Hash Abierto (Encadenamiento o "Chaining"): En este estilo de hash, las posiciones del arreglo no guardan los elementos directamente, sino que contienen punteros a otras estrucuturas (listas anlazadas, árboles, etc). Si ocurre una colisión, el nuevo par clave-valor simplemente se encadena como un elemento adicional en la estructura correspondiente a ese índice.

Hash Cerrado: Todos los elementos se almacenan directamente dentro del mismo arreglo, sin usar estructuras dinámicas externas. Cuando hay una colisión, el algoritmo debe buscar otra celda disponible dentro de la tabla mediante diferentes métodos de búsqueda:

Probing lineal: Consiste en buscar el siguiente espacio libre inmediato, avanzando secuencialmente. Es el método que utilizamos en este trabajo.

Probing cuadrático: Calcula el salto hacia la próxima celda basándose en los intentos fallidos al cuadrado ((intentos\ fallidos)^2) para intentar insertar. Si bien produce una mejora, es mínima por lo que en muchos casos no resulta mejor. 

Hash doble: Consiste en aplicar una segunda función de hash a la clave cuando se produce una colisión, usando ese nuevo resultado para calcular el tamaño del salto. Tampoco lo implementamos, ya que la mejora es mínima y hasta puede resultar peor.

### 5.5 ¿Por qué es importante el tamaño de la tabla (tanto para tablas abiertas como cerradas)

El tamaño de la tabla es muy importante porque determina el factor de carga (la proporción entre elementos insertados y la capacidad total), lo cual impacta directamente en el rendimiento de la estructura:

En tablas cerradas: Si la tabla tiene muy poca capacidad, el factor de carga es elevado por lo que las colisiones se producen de manera seguida, generando grandes agrupamientos de datos (clusters). Esto causa que la complejidad de las operaciones cercanas o iguales a O(1) se conviertan en O(n), ya que el probing obliga a recorrer muchas celdas. Si la tabla se llena por completo, se vuelve imposible insertar nuevos elementos sin redimensionar.

En tablas abiertas (encadenamiento): Aunque no existe un límite físico de inserción porque se usan estructuras dinámicas externas, un tamaño de tabla muy chico provoca que muchas claves colisionen en unos pocos índices. Esto genera que las estructuras externas sean extremadamente largas, convirtiendo la búsqueda O(1) en una simple búsqueda lineal O(n).

En resumen, el tamaño debe mantener un equilibrio: lo suficientemente grande para minimizar las colisiones y garantizar el acceso cercano o igual a O(1), pero lo suficientemente ajustado para no desperdiciar grandes bloques de memoria innecesariamente.

### 5.6 Dado que en una tabla abierta se pueden encadenar colisiones sin importar el tamaño de la tabla, ¿Realmente importa el tamaño?

Sí, el tamaño de la tabla importa. Aunque el encadenamiento permite resolver las colisiones añadiendo nuevos elementos a las estructuras externas, no darle importancia al tamaño del arreglo principal impacta directamente en la eficiencia temporal. Si la tabla es demasiado pequeña en relación con la cantidad de datos almacenados, el factor de carga será muy alto y múltiples claves colisionarían.

Como resultado, las estructuras externas crecerán de forma desproporcionada. Cuando se necesite buscar, actualizar o eliminar un elemento, el algoritmo estará obligado a recorrer linealmente una estructura gigantesca, resultando en una complejidad temporal de las operaciones de  O(n) en vez de O(1). 