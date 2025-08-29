#ifndef CHECKPOINTS_H
#define CHECKPOINTS_H
/*
Sobre Ifndef
======
ifndef es una construcción de pre compilación que sólo incorpora el texto entre "#ifndef SYMBOL ... #endif sólo si el símbolo SYMBOL aún no fue definido
se suele utilizar esta combinación de
#ifndef SYMBOL
#define SYMBOL
para que el pre-procesador sólo evalúe una vez los encabezados antes de comenzar la compilación

Sobre includes
==============
Recuerden que los include son directivas que incluyen,
en este caso, las definiciones y encabezados de las funciones de uso común que queremos
utilizar en nuestro programa
*/
#include <stdio.h>   //encabezado de funciones de entrada y salida fopen, fclose, fgetc, printf, fprintf ...
#include <stdlib.h>  //biblioteca estándar, atoi, atof, rand, srand, abort, exit, system, NULL, malloc, calloc, realloc...
#include <stdint.h>  //contiene la definición de tipos enteros ligados a tamaños int8_t, int16_t, uint8_t,...
#include <ctype.h>   //contiene funciones relacionadas a caracteres, isdigit, islower, tolower...
#include <string.h>  //contiene las funciones relacionadas a strings, memcmp, strcat, memset, memmove, strlen,strstr...
#include <math.h>    //define funciones matemáticas como cos, sin, abs, sqrt, log...
#include <stdbool.h> //contiene las definiciones de datos booleanos, true (1), false (0)
#include <unistd.h>  //define constantes y tipos standard, NULL, R_OK, F_OK, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO...
#include <assert.h>  //provee la macro assert que evalúa una condición, y si no se cumple provee información diagnóstica y aborta la ejecución

//****************************************
// Declaración de funciones
//****************************************

/* String */

// Compara dos strings en orden lexicográfico. Ver https://es.wikipedia.org/wiki/Orden_lexicografico.
// Debe retornar:
// 0 si son iguales
// 1 si a < b
//-1 si a > b
int32_t strCmp(char *a, char *b);

// Genera una copia del string pasado por parámetro. El puntero pasado siempre es válido
// aunque podría corresponderse a la cadena vacía.
char *strClone(char *a);

// Borra el string pasado por parámetro. Esta función es equivalente a la función free.
void strDelete(char *a);

// Escribe el string en el stream indicado a través de pFile. Si el string es vacío debe escribir "NULL"
void strPrint(char *a, FILE *pFile);

// Retorna la cantidad de caracteres distintos de cero del \emph{string} pasado por parámetro.
uint32_t strLen(char *a);

#endif