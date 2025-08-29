#pragma once
#include <stdio.h> 		//encabezado de funciones de entrada y salida fopen, fclose, fgetc, printf, fprintf ...
#include <stdlib.h>		//biblioteca estándar, atoi, atof, rand, srand, abort, exit, system, NULL, malloc, calloc, realloc...
#include <stdint.h>		//contiene la definición de tipos enteros ligados a tamaños int8_t, int16_t, uint8_t,...
#include <ctype.h>		//contiene funciones relacionadas a caracteres, isdigit, islower, tolower...
#include <string.h>		//contiene las funciones relacionadas a strings, memcmp, strcat, memset, memmove, strlen,strstr...
#include <math.h>		//define funciones matemáticas como cos, sin, abs, sqrt, log...
#include <stdbool.h>	//contiene las definiciones de datos booleanos, true (1), false (0)
#include <unistd.h>		//define constantes y tipos standard, NULL, R_OK, F_OK, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO...
#include <assert.h>		//provee la macro assert que evalúa una condición, y si no se cumple provee información diagnóstica y aborta la ejecución

//*************************************
//Declaración de estructuras
//*************************************

typedef struct {
	uint32_t id; //asmdef_offset:USUARIO_ID_OFFSET
	uint32_t nivel; //asmdef_offset:USUARIO_NIVEL_OFFSET
} usuario_t; //asmdef_size:USUARIO_SIZE

typedef struct {
	char categoria[3]; //asmdef_offset:CASO_CATEGORIA_OFFSET
	uint16_t estado; //asmdef_offset:CASO_ESTADO_OFFSET
	usuario_t* usuario; //asmdef_offset:CASO_USUARIO_OFFSET
} caso_t; //asmdef_size:CASO_SIZE

typedef struct {
	caso_t* casos_nivel_0; //asmdef_offset:SEGMENTACION_CASOS0_OFFSET
	caso_t* casos_nivel_1; //asmdef_offset:SEGMENTACION_CASOS1_OFFSET
	caso_t* casos_nivel_2; //asmdef_offset:SEGMENTACION_CASOS2_OFFSET
} segmentacion_t; //asmdef_size:SEGMENTACION_SIZE

typedef struct {
	uint8_t cantidad_CLT; //asmdef_offset:ESTADISTICAS_CLT_OFFSET
	uint8_t cantidad_RBO; //asmdef_offset:ESTADISTICAS_RBO_OFFSET
	uint8_t cantidad_KSC; //asmdef_offset:ESTADISTICAS_KSC_OFFSET
	uint8_t cantidad_KDT; //asmdef_offset:ESTADISTICAS_KDT_OFFSET
	uint8_t cantidad_estado_0; //asmdef_offset:ESTADISTICAS_ESTADO0_OFFSET
	uint8_t cantidad_estado_1; //asmdef_offset:ESTADISTICAS_ESTADO1_OFFSET
	uint8_t cantidad_estado_2; //asmdef_offset:ESTADISTICAS_ESTADO2_OFFSET
} estadisticas_t; //asmdef_size:ESTADISTICAS_SIZE


typedef uint16_t funcionCierraCasos_t(caso_t* caso);

//*******************************
//Declaración de funciones
//*******************************

/*Segmentar los casos segun nivel del usuario*/
segmentacion_t* segmentar_casos(caso_t* arreglo_casos, int largo);

/*Cerrar casos segun prioridad*/
void resolver_automaticamente(funcionCierraCasos_t* funcion, caso_t* arreglo_casos, caso_t* casos_a_revisar, int largo);

estadisticas_t* calcular_estadisticas(caso_t* arreglo_casos, int largo, uint32_t usuario_id);
