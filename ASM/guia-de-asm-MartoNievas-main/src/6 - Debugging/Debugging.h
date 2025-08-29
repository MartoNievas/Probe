#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "structs.h" //define estructuras de datos de los ejercicios

// Marca el cada ejercicio como hecho (`true`) o pendiente (`false`).
extern bool EJERCICIO_1_HECHO;
extern bool EJERCICIO_2_HECHO;
extern bool EJERCICIO_3_HECHO;
extern bool EJERCICIO_4_HECHO;

// OPCIONAL: implementar en C
 
uint64_t ejercicio1(uint64_t sum1, uint64_t sum2, uint64_t sum3, uint64_t sum4, uint64_t sum5);

void ejercicio2(item_t* un_item, uint32_t id, uint32_t cantidad, char nombre[]);

uint32_t ejercicio3(uint32_t* array, uint32_t size, uint32_t (*fun_ej_3)(uint32_t a, uint32_t b));

uint32_t* ejercicio4(uint32_t** array, uint32_t size, uint32_t constante);