#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

#include "../test-utils.h"
#include "Memoria.h"

#define ARR_LENGTH  4
#define ROLL_LENGTH 10

static uint32_t x[ROLL_LENGTH];
static double   f[ROLL_LENGTH];

/**
 * Tests Memoria
 */

 TEST(test_strLen) {
	TEST_ASSERT_EQUALS(uint32_t,  0, TEST_CALL_I(strLen, ""));
	TEST_ASSERT_EQUALS(uint32_t,  3, TEST_CALL_I(strLen, "sar"));
	TEST_ASSERT_EQUALS(uint32_t,  2, TEST_CALL_I(strLen, "23"));
	TEST_ASSERT_EQUALS(uint32_t,  4, TEST_CALL_I(strLen, "taaa"));
	TEST_ASSERT_EQUALS(uint32_t,  3, TEST_CALL_I(strLen, "tbb"));
	TEST_ASSERT_EQUALS(uint32_t,  3, TEST_CALL_I(strLen, "tix"));
	TEST_ASSERT_EQUALS(uint32_t,  5, TEST_CALL_I(strLen, "taaab"));
	TEST_ASSERT_EQUALS(uint32_t,  4, TEST_CALL_I(strLen, "taa0"));
	TEST_ASSERT_EQUALS(uint32_t,  3, TEST_CALL_I(strLen, "tbb"));
	TEST_ASSERT_EQUALS(uint32_t, 11, TEST_CALL_I(strLen, "Hola mundo!"));
	TEST_ASSERT_EQUALS(uint32_t,  9, TEST_CALL_I(strLen, "Astronomo"));
	TEST_ASSERT_EQUALS(uint32_t, 10, TEST_CALL_I(strLen, "Astrognomo"));
	TEST_ASSERT_EQUALS(uint32_t, 19, TEST_CALL_I(strLen, "Campeones del mundo"));
}

TEST(test_strClone_string_normal) {
	char* a = "Omega 4";
	char* ac = TEST_CALL_S(strClone, a);
	TEST_ASSERT(a != ac);
	strcpy(assert_name, "ac == \"Omega 4\"");
	TEST_ASSERT(ac[0] == 'O' && ac[1] == 'm' && ac[2] == 'e' && ac[3] == 'g' && ac[4] == 'a' && ac[5] == ' ' && ac[6] == '4' && ac[7] == '\0');

	strDelete(ac);
}

TEST(test_strClone_string_vacio) {
	char* n = "";
	char* nc = TEST_CALL_S(strClone, n);
	TEST_ASSERT(n != nc);
	strcpy(assert_name, "ac == \"\"");
	TEST_ASSERT(nc[0] == '\0');

	strDelete(nc);
}

TEST(test_strCmp_las_cadenas_que_son_iguales_son_iguales) {
	// Esta cadena vive en la pila
	char cadena[] = "Orga 2!";
	TEST_ASSERT_EQUALS(int32_t, 0, TEST_CALL_I(strCmp, "Orga 2!", cadena));
	TEST_ASSERT_EQUALS(int32_t, 0, TEST_CALL_I(strCmp, cadena,    "Orga 2!"));
	TEST_ASSERT_EQUALS(int32_t, 0, TEST_CALL_I(strCmp, "Omega 4", "Omega 4"));
	TEST_ASSERT_EQUALS(int32_t, 0, TEST_CALL_I(strCmp, "",        ""));
	TEST_ASSERT_EQUALS(int32_t, 0, TEST_CALL_I(strCmp, "Palaven", "Palaven"));
	TEST_ASSERT_EQUALS(int32_t, 0, TEST_CALL_I(strCmp, "Feros",   "Feros"));
}

TEST(test_strCmp_vacio_es_menor_a_todo) {
	// Esta cadena vive en la pila
	char cadena[] = "Orga 2!";
	TEST_ASSERT_EQUALS(int32_t, 1, TEST_CALL_I(strCmp, "", cadena));
	TEST_ASSERT_EQUALS(int32_t, 1, TEST_CALL_I(strCmp, "", "Omega 4"));
	TEST_ASSERT_EQUALS(int32_t, 1, TEST_CALL_I(strCmp, "", "Feros"));
	TEST_ASSERT_EQUALS(int32_t, 1, TEST_CALL_I(strCmp, "", "Palaven"));
}

TEST(test_strCmp_hay_cadenas_menores_a_otras) {
	// Esta cadena vive en la pila
	char cadena[] = "Orga 2!";
	TEST_ASSERT_EQUALS(int32_t, 1, TEST_CALL_I(strCmp, "Omega 4",    cadena));
	TEST_ASSERT_EQUALS(int32_t, 1, TEST_CALL_I(strCmp, cadena,       "Orga 3?"));
	TEST_ASSERT_EQUALS(int32_t, 1, TEST_CALL_I(strCmp, "Feros",      "Omega 4"));
	TEST_ASSERT_EQUALS(int32_t, 1, TEST_CALL_I(strCmp, "Feros",      "Palaven"));
	TEST_ASSERT_EQUALS(int32_t, 1, TEST_CALL_I(strCmp, "Omega 4",    "Palaven"));
	TEST_ASSERT_EQUALS(int32_t, 1, TEST_CALL_I(strCmp, "Astrognomo", "Astronomo")); // Obviamente, porque un astro-gnomo va a ser más chiquitito
}

TEST(test_strCmp_todo_es_mayor_a_vacio) {
	// Esta cadena vive en la pila
	char cadena[] = "Orga 2!";
	TEST_ASSERT_EQUALS(int32_t, -1, TEST_CALL_I(strCmp, cadena,    ""));
	TEST_ASSERT_EQUALS(int32_t, -1, TEST_CALL_I(strCmp, "Omega 4", ""));
	TEST_ASSERT_EQUALS(int32_t, -1, TEST_CALL_I(strCmp, "Feros",   ""));
	TEST_ASSERT_EQUALS(int32_t, -1, TEST_CALL_I(strCmp, "Palaven", ""));
}


TEST(test_strCmp_hay_cadenas_mayores_a_otras) {
	// Esta cadena vive en la pila
	char cadena[] = "Orga 2!";
	TEST_ASSERT_EQUALS(int32_t, -1, TEST_CALL_I(strCmp, cadena,      "Omega 4"));
	TEST_ASSERT_EQUALS(int32_t, -1, TEST_CALL_I(strCmp, "Orga 3?",   cadena));
	TEST_ASSERT_EQUALS(int32_t, -1, TEST_CALL_I(strCmp, "Omega 4",   "Feros"));
	TEST_ASSERT_EQUALS(int32_t, -1, TEST_CALL_I(strCmp, "Palaven",   "Feros"));
	TEST_ASSERT_EQUALS(int32_t, -1, TEST_CALL_I(strCmp, "Palaven",   "Omega 4"));
	TEST_ASSERT_EQUALS(int32_t, -1, TEST_CALL_I(strCmp, "Astronomo", "Astrognomo")); // Obviamente, porque un astro-gnomo va a ser más chiquitito
}

TEST(test_strCmp_stress_test) {
	char* cadenas[9] = {"sar","23","taaa","tbb","tix", "taaab", "taa0", "tbb", ""};
	int32_t resultados[9][9] = {
		{  0, -1,  1,  1,  1,  1,  1,  1, -1 },
		{  1,  0,  1,  1,  1,  1,  1,  1, -1 },
		{ -1, -1,  0,  1,  1,  1, -1,  1, -1 },
		{ -1, -1, -1,  0,  1, -1, -1,  0, -1 },
		{ -1, -1, -1, -1,  0, -1, -1, -1, -1 },
		{ -1, -1, -1,  1,  1,  0, -1,  1, -1 },
		{ -1, -1,  1,  1,  1,  1,  0,  1, -1 },
		{ -1, -1, -1,  0,  1, -1, -1,  0, -1 },
		{  1,  1,  1,  1,  1,  1,  1,  1,  0 },
	};

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			sprintf(assert_name, "strCmp(\"%s\", \"%s\")", cadenas[i], cadenas[j]);
			TEST_ASSERT_EQUALS(int32_t, resultados[i][j], TEST_CALL_I(strCmp, cadenas[i], cadenas[j]));
		}
	}
}

int main() {
	srand(0);

    char test_suite_name[] = "Memoria";

	printf("==============\n");
	printf("= %s\n", test_suite_name);
	printf("==============\n");
	test_strLen();
	test_strClone_string_normal();
	test_strClone_string_vacio();
	test_strCmp_las_cadenas_que_son_iguales_son_iguales();
	test_strCmp_vacio_es_menor_a_todo();
	test_strCmp_hay_cadenas_menores_a_otras();
	test_strCmp_todo_es_mayor_a_vacio();
	test_strCmp_hay_cadenas_mayores_a_otras();
	test_strCmp_stress_test();
	printf("\n");

	tests_end(test_suite_name);
	return 0;
}
