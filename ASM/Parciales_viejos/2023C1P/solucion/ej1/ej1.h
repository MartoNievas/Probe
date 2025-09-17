#ifndef EJ1_H
#define EJ1_H

#include <assert.h> //provee la macro assert que evalúa una condición, y si no se cumple provee información diagnóstica y aborta la ejecución
#include <ctype.h> //contiene funciones relacionadas a caracteres, isdigit, islower, tolower...
#include <math.h> //define funciones matemáticas como cos, sin, abs, sqrt, log...
#include <stdbool.h> //contiene las definiciones de datos booleanos, true (1), false (0)
#include <stdint.h> //contiene la definición de tipos enteros ligados a tamaños int8_t, int16_t, uint8_t,...
#include <stdio.h> //encabezado de funciones de entrada y salida fopen, fclose, fgetc, printf, fprintf ...
#include <stdlib.h> //biblioteca estándar, atoi, atof, rand, srand, abort, exit, system, NULL, malloc, calloc, realloc...
#include <string.h> //contiene las funciones relacionadas a strings, memcmp, strcat, memset, memmove, strlen,strstr...
#include <unistd.h> //define constantes y tipos standard, NULL, R_OK, F_OK, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO...

//*************************************
// Declaración de estructuras
//*************************************

#define USE_ASM_IMPL 1

typedef struct {
  uint8_t colum_largo; // offset 0
  char *nombre;        // offset 8
  uint8_t colum_corto; // offset 16
} templo;              // tamanio 16 bytes

//*******************************
// Declaración de funciones de ej1
//*******************************

uint32_t cuantosTemplosClasicos(templo *temploArr, size_t temploArr_len);
uint32_t cuantosTemplosClasicos_c(templo *temploArr, size_t temploArr_len);

templo *templosClasicos(templo *temploArr, size_t temploArr_len);
templo *templosClasicos_c(templo *temploArr, size_t temploArr_len);

#endif
