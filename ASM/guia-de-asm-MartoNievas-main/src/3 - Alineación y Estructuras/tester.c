#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

#include "../test-utils.h"
#include "Estructuras.h"

#define ARR_LENGTH  4
#define ROLL_LENGTH 10

static uint32_t x[ROLL_LENGTH];
static double   f[ROLL_LENGTH];

void shuffle(uint32_t max){
	for (int i = 0; i < ROLL_LENGTH; i++) {
		x[i] = (uint32_t) rand() % max;
        	f[i] = ((float)rand()/(float)(RAND_MAX)) * max;
	}
}

/**
 * Tests Estructuras
 */

 TEST(test_cantidad_total_de_elementos) {
	nodo_t* array[ARR_LENGTH];

	for (int i = 0; i < 100; i++) {
		shuffle(1000);
		sprintf(assert_name, "cantidad_total_elementos (prueba %d)", i);

		for(int j=0; j<ARR_LENGTH; j++){
			array[j] = calloc(1, sizeof(nodo_t));
			array[j]->longitud = x[j];
		}

		for(int j=0; j<ARR_LENGTH-1; j++){
			array[j]->next = array[j+1];
		}

		lista_t lista;
		lista.head = array[0];

		nodo_t* nodo = lista.head;
		uint32_t result = 0;

		while(nodo != NULL){
			result += nodo->longitud;
			nodo = nodo->next;
		}

		TEST_ASSERT_EQUALS(uint32_t, result, TEST_CALL_I(cantidad_total_de_elementos, &lista));

		for(int j=0; j<ARR_LENGTH; j++){
			free(array[j]);
		}
	}
}

TEST(test_cantidad_total_de_elementos_packed) {
	packed_nodo_t* array[ARR_LENGTH];

	for (int i = 0; i < 100; i++) {
		shuffle(1000);
		sprintf(assert_name, "cantidad_total_elementos_packed (prueba %d)", i);

		for(int j=0; j<ARR_LENGTH; j++){
			array[j] = calloc(1, sizeof(packed_nodo_t));
			array[j]->longitud = x[j];
		}

		for(int j=0; j<ARR_LENGTH-1; j++){
			array[j]->next = array[j+1];
		}

		packed_lista_t lista;
		lista.head = array[0];

		packed_nodo_t* nodo = lista.head;
		uint32_t result = 0;

		while(nodo != NULL){
			result += nodo->longitud;
			nodo = nodo->next;
		}

		TEST_ASSERT_EQUALS(uint32_t, result, TEST_CALL_I(cantidad_total_de_elementos_packed, &lista));

		for(int j=0; j<ARR_LENGTH; j++){
			free(array[j]);
		}
	}
}


int main() {
	srand(0);

    char test_suite_name[] = "Estructuras";

	printf("==============\n");
	printf("= %s\n", test_suite_name);
	printf("==============\n");
	test_cantidad_total_de_elementos();
	test_cantidad_total_de_elementos_packed();
	printf("\n");

	tests_end(test_suite_name);
	return 0;
}
