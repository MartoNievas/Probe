#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "ejs.h"

int main (void){
	// // //Crear array con capacidad 5
    // uint8_t cap = 5;
    // str_array_t* arr = strArrayNew(cap);
    // assert(arr != NULL);                        // El puntero no debe ser NULL
    // assert(arr->size == 0);                     // Debe iniciar con tamaño 0
    // assert(arr->capacity == cap);              // La capacidad debe ser la que se pasó
    // assert(arr->data != NULL); 
	// free(arr->data);
    // free(arr);

	// str_array_t* arr = strArrayNew(5);
    // assert(strArrayGetSize(arr) == 0);  // Al principio debe ser 0
    // free(arr->data);
    // free(arr);
    // printf("✔ test_strArrayGetSize_initial_size passed\n");

	// // Crear un array y agregar un elemento de ejemplo
	// str_array_t* arr1 = strArrayNew(5);
	// arr1->size = 1;  // Simulamos agregar un elemento
	// assert(strArrayGetSize(arr1) == 1);  // El tamaño debe ser 1
	// free(arr1->data);
	// free(arr1);
	// printf("✔ test_strArrayGetSize_after_addition passed\n");

	// str_array_t* arr2 = strArrayNew(5);
	// arr2->size = 3;  // Simulamos agregar tres elementos
	// assert(strArrayGetSize(arr2) == 3);  // El tamaño debe ser 3
	// free(arr2->data);
	// free(arr2);
	// printf("✔ test_strArrayGetSize_after_multiple_additions passed\n");

	str_array_t* arr = strArrayNew(2); // Crear un array con capacidad de 2
    strArrayAddLast(arr, "Hello");
    strArrayAddLast(arr, "World");

    // Verificar que los strings estén correctamente almacenados
    assert(strcmp(arr->data[0], "Hello") == 0);
    assert(strcmp(arr->data[1], "World") == 0);

    // Agregar un elemento más y verificar la expansión
    strArrayAddLast(arr, "Test");
    assert(strArrayGetSize(arr) == 3); // El tamaño debe ser 3

    // Verificar que el nuevo string esté correctamente almacenado
    assert(strcmp(arr->data[2], "Test") == 0);

    free(arr->data[0]);
    free(arr->data[1]);
    free(arr->data[2]);
    free(arr->data);
    free(arr);


	return 0;
}


