#pragma once
#include <assert.h>  //provee la macro assert que evalúa una condición, y si no se cumple provee información diagnóstica y aborta la ejecución
#include <ctype.h>   //contiene funciones relacionadas a caracteres, isdigit, islower, tolower...
#include <math.h>    //define funciones matemáticas como cos, sin, abs, sqrt, log...
#include <stdbool.h> //contiene las definiciones de datos booleanos, true (1), false (0)
#include <stdint.h>  //contiene la definición de tipos enteros ligados a tamaños int8_t, int16_t, uint8_t,...
#include <stdio.h>   //encabezado de funciones de entrada y salida fopen, fclose, fgetc, printf, fprintf ...
#include <stdlib.h>  //biblioteca estándar, atoi, atof, rand, srand, abort, exit, system, NULL, malloc, calloc, realloc...
#include <string.h>  //contiene las funciones relacionadas a strings, memcmp, strcat, memset, memmove, strlen,strstr...
#include <unistd.h>  //define constantes y tipos standard, NULL, R_OK, F_OK, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO...

//*************************************
// Declaración de estructuras del marketplace Orga2-Libre
//*************************************
// Estructura Usuario
typedef struct
{
  uint32_t id;   // asmdef_offset:USUARIO_ID_OFFSET
  uint8_t nivel; // asmdef_offset:USUARIO_NIVEL_OFFSET
} usuario_t;     // asmdef_size:USUARIO_SIZE

// Estructura Item
typedef struct
{
  usuario_t *usuario; // asmdef_offset:PRODUCTO_USUARIO_OFFSET
  char categoria[9];  // asmdef_offset:PRODUCTO_CATEGORIA_OFFSET
  char nombre[25];    // asmdef_offset:PRODUCTO_NOMBRE_OFFSET
  uint16_t estado;    // asmdef_offset:PRODUCTO_ESTADO_OFFSET
  uint32_t precio;    // asmdef_offset:PRODUCTO_PRECIO_OFFSET
  uint32_t id;        // asmdef_offset:PRODUCTO_ID_OFFSET
} producto_t;         // asmdef_size:PRODUCTO_SIZE

// Estructura Nodo
typedef struct publicacion_t
{
  struct publicacion_t *next; // asmdef_offset:PUBLICACION_NEXT_OFFSET
  producto_t *value;          // asmdef_offset:PUBLICACION_VALUE_OFFSET
} publicacion_t;              // asmdef_size:PUBLICACION_SIZE

// Estructura Head
typedef struct
{
  struct publicacion_t *first; // asmdef_offset:CATALOGO_FIRST_OFFSET
} catalogo_t;                  // asmdef_size:CATALOGO_SIZE

//*******************************
// Declaración de funciones
//*******************************

// Ejercicio 1:
producto_t *filtrarPublicacionesNuevasDeUsuariosVerificados(catalogo_t *h);

// Ejercicio 2:
catalogo_t *removerCopias(catalogo_t *h);

// Ejercicio 3:
usuario_t **asignarNivelesParaNuevosUsuarios(uint32_t *ids,
                                             uint32_t cantidadDeIds,
                                             uint8_t (*deQueNivelEs)(uint32_t));
