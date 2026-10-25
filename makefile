CC = gcc
CFLAGS = -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0 -g
CFLAGS_TEST = -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0 -g -I. -Ipruebas -Isrc
VALGRIND_FLAGS = --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all
SRC = src/*.c

.PHONY: main pruebas valgrind clean

main: main.c $(SRC)
	$(CC) $(CFLAGS) $(SRC) main.c -o tp

pruebas: pruebas/pruebas_alumno.c pruebas/pa2m.c src/diccionario.c
	$(CC) $(CFLAGS_TEST) src/diccionario.c pruebas/pa2m.c pruebas/pruebas_alumno.c -o pruebas_alumno
	./pruebas_alumno

valgrind: pruebas
	valgrind $(VALGRIND_FLAGS) ./pruebas_alumno

clean:
	rm -f tp pruebas_alumno *.o