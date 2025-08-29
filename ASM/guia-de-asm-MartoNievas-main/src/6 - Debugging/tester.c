#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../test-utils.h"
#include "Debugging.h"

// Cuenta cuántos tests corrieron exitosamente.
uint64_t successful_tests = 0;

//Cuenta cuántos tests test fallaron.
uint64_t failed_tests = 0;

//El mensaje [DONE] escrito en verde.
#define DONE "[\033[32;1mDONE\033[0m] "

// El mensaje [FAIL] escrito en rojo.
#define FAIL "[\033[31;1mFAIL\033[0m] "

// El mensaje [INFO] escrito en amarillo.
#define INFO "[\033[33;1mINFO\033[0m] "

// El mensaje [SKIP] escrito en magenta.
#define SKIP "[\033[95;1mSKIP\033[0m] "

uint32_t fun_ej_3(uint32_t a, uint32_t b) {
    uint32_t res = (a + 1) * b + 10;
    return res;
}

void testResultMessage(bool comparacion, char* testName){
	if (comparacion) {
		successful_tests++;
		printf(DONE "%s: el resultado esperado y el obtenido coinciden.\n",testName);
	} else {
		failed_tests++;		
		printf(FAIL "%s: el resultado esperado y el obtenido no son iguales.\n", testName);
	}
}

void test_1_sumas(){
	uint64_t resultado_suma = ejercicio1(0,0,0,0,0);
	testResultMessage(0 == resultado_suma, "test 1.1");
	
	resultado_suma = ejercicio1(75,0,0,0,0);
	testResultMessage(75 == resultado_suma, "test 1.2");

	resultado_suma = ejercicio1(184,0,25,0,0);
	testResultMessage(209 == resultado_suma, "test 1.3");

	resultado_suma = ejercicio1(5,0,10,5,0);
	testResultMessage(20 == resultado_suma, "test 1.4");

	resultado_suma = ejercicio1(1,2,220,7,0);
	testResultMessage(230 == resultado_suma, "test 1.5");

	resultado_suma = ejercicio1(100,101,102,103,104);
	testResultMessage(510 == resultado_suma, "test 1.6");

	resultado_suma = ejercicio1(4294967299,1,20,1,21);
	testResultMessage(4294967342 == resultado_suma, "test 1.7");

	resultado_suma = ejercicio1(4294967295,4294967295,4294967295,4294967295,4294967295);
	testResultMessage(21474836475 == resultado_suma, "test 1.8");
}

void test_ej_1() {
	uint64_t successful_at_start = successful_tests;
	uint64_t failed_at_start = failed_tests;

	if (!EJERCICIO_1_HECHO) {
		printf(SKIP "El ejercicio 1 no está hecho aún.\n");
		return;
	}
	
	test_1_sumas();
	if (failed_at_start < failed_tests) {
		printf(FAIL "El ejercicio 1 tuvo tests que fallaron.\n");
	}
}


bool items_iguales(item_t* a, item_t* b){
	bool son_iguales = true;
	son_iguales = son_iguales && (a->cantidad == b->cantidad);
	son_iguales = son_iguales && (a->id == b->id);
	son_iguales = son_iguales && (strcmp(a->nombre, b->nombre) == 0);
	return son_iguales;
}

void test_2_struct(){
	item_t* un_item = malloc(sizeof(item_t));
	item_t* esperado = malloc(sizeof(item_t));
	
	strcpy(esperado->nombre, "poncho");
	esperado->id = 1234;
	esperado->cantidad = 3;

	ejercicio2(un_item, 1234, 3, "poncho");
	
	testResultMessage(items_iguales(esperado, un_item), "test 2.1");

	free(un_item);
	free(esperado);
}

void test_2_struct_borde(){
	item_t* un_item = malloc(sizeof(item_t));
	item_t* esperado = malloc(sizeof(item_t));
	
	strcpy(esperado->nombre, "cortinas");
	esperado->id = 4294967295;
	esperado->cantidad = 4294967295;

	ejercicio2(un_item, 4294967295, 4294967295, "cortinas");
	
	testResultMessage(items_iguales(esperado, un_item), "test 2.2");

	free(un_item);
	free(esperado);
}

void test_ej_2() {
	uint64_t successful_at_start = successful_tests;
	uint64_t failed_at_start = failed_tests;
	
	if (!EJERCICIO_2_HECHO) {
		printf(SKIP "El ejercicio 2 no está hecho aún.\n");
		return;
	}

	test_2_struct();
	test_2_struct_borde();

	if (failed_at_start < failed_tests) {
		printf(FAIL "El ejercicio 2 tuvo tests que fallaron.\n");
	}
}


void test_3_vacio(){
	uint32_t res = 0;
	res = ejercicio3(NULL, 0, fun_ej_3);
	testResultMessage(64 == res, "test 3.1");
}

void test_3_un_elemento(){
	uint32_t res = 0;
	uint32_t array[] = {2};
	res = ejercicio3(array, 1, fun_ej_3);
	testResultMessage(12 == res, "test 3.2");
}

void test_3_dos_elementos(){
	uint32_t res = 0;
	uint32_t array[] = {2,3};
	res = ejercicio3(array, 2, fun_ej_3);
	testResultMessage(61 == res, "test 3.3");
}

void test_3_cinco_elementos(){
	uint32_t res = 0;
	uint32_t array[] = {1,1,1,1,1};
	res = ejercicio3(array, 5, fun_ej_3);
	testResultMessage(341 == res, "test 3.4");
}

void test_ej_3(){
	uint64_t successful_at_start = successful_tests;
	uint64_t failed_at_start = failed_tests;
	if (!EJERCICIO_3_HECHO) {
		printf(SKIP "El ejercicio 3 no está hecho aún.\n");
		return;
	}

	test_3_vacio();
	test_3_un_elemento();
	test_3_dos_elementos();
	test_3_cinco_elementos();

	if (failed_at_start < failed_tests) {
		printf(FAIL "El ejercicio 3 tuvo tests que fallaron.\n");
	}
}

void testResults4(uint32_t* esperado, uint32_t* resultado, uint32_t* inicial[], uint32_t size, char* testName) {
	bool iguales =true;

	for (uint32_t i = 0; i < size; i++)
	{
		if(esperado[i] != resultado[i]){
			iguales = false;
		}
	}
	
	for (uint32_t i = 0; i < size; i++)
	{
		if(inicial[i] != NULL){
			iguales = false;
		}
	}

	testResultMessage(iguales, testName);
}


void test_4_un_elemento(){
	uint32_t* array_inicial[1];

	uint32_t* punteroUint = malloc(sizeof(uint32_t));
	*punteroUint = 10;
	array_inicial[0] = punteroUint;
	
	uint32_t array_res[] = {80};
	uint32_t* array = ejercicio4(array_inicial, 1, 8);
	testResults4(array_res, array, array_inicial, 1, "test 4.1");
	free(array);
}

void test_4_tres_elementos(){
	uint32_t* array_inicial[3];

	for(int i=0; i<3; i++){
		uint32_t* punteroUint = malloc(sizeof(uint32_t));
		*punteroUint = (i+1)*5;
		array_inicial[i] = punteroUint;
	}
	
	uint32_t array_res[] = {25,50,75};
	uint32_t* array = ejercicio4(array_inicial, 3, 5);
	testResults4(array_res, array, array_inicial, 3, "test 4.2");
	free(array);
}

void test_ej_4(){
	uint64_t successful_at_start = successful_tests;
	uint64_t failed_at_start = failed_tests;
	if (!EJERCICIO_4_HECHO) {
		printf(SKIP "El ejercicio 4 no está hecho aún.\n");
		return;
	}

    test_4_un_elemento();
	test_4_tres_elementos();

	if (failed_at_start < failed_tests) {
		printf(FAIL "El ejercicio 4 tuvo tests que fallaron.\n");
	}
}

int main(int argc, char* argv[]) {
	test_ej_1();
	test_ej_2();
	test_ej_3();
	test_ej_4();

	printf(
		"\nSe corrieron %lu tests. %lu corrieron exitosamente. %lu fallaron.\n",
		failed_tests + successful_tests, successful_tests, failed_tests
	);

	if (failed_tests || !EJERCICIO_1_HECHO || !EJERCICIO_2_HECHO || !EJERCICIO_3_HECHO || !EJERCICIO_4_HECHO) {
		return 1;
	} else {
		return 0;
	}
}
