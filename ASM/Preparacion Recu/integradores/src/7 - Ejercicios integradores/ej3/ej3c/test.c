#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../tests-aux.h"
#include "../ejs.h"

/**
 * Cuenta cuántos tests corrieron exitosamente.
 */
uint64_t successful_tests = 0;
/**
 * Cuenta cuántos tests test fallaron.
 */
uint64_t failed_tests = 0;

/**
 * Variables para controlar qué ejercicios están hechos
 */
bool EJERCICIO_3_HECHO = true;  // resolver_automaticamente

/**
 * Función auxiliar para crear un caso de prueba
 */
caso_t crear_caso(uint32_t id, uint32_t nivel, const char* categoria, uint16_t estado) {
    caso_t caso;
    strcpy(caso.categoria, categoria);
    caso.estado = estado;
    caso.usuario = malloc(sizeof(usuario_t));
    caso.usuario->id = id;
    caso.usuario->nivel = nivel;
    return caso;
}

/**
 * Función auxiliar para liberar memoria de un arreglo de casos
 */
void liberar_casos(caso_t* casos, int largo) {
    for (int i = 0; i < largo; i++) {
        free(casos[i].usuario);
    }
}

bool casos_iguales(caso_t caso1, caso_t caso2){
    return strcmp(caso1.categoria, caso2.categoria) == 0 &&
           caso1.estado == caso2.estado &&
           caso1.usuario->id == caso2.usuario->id &&
           caso1.usuario->nivel == caso2.usuario->nivel;

}

estadisticas_t crear_estadistica(uint8_t CLT,uint8_t RBO,uint8_t KSC,uint8_t KDT,uint8_t estado_0,uint8_t estado_1,uint8_t estado_2){
    estadisticas_t estadistica;
    estadistica.cantidad_CLT = CLT;
    estadistica.cantidad_RBO = RBO;
    estadistica.cantidad_KSC = KSC;
    estadistica.cantidad_KDT = KDT;
    estadistica.cantidad_estado_0 = estado_0;
    estadistica.cantidad_estado_1 = estado_1;
    estadistica.cantidad_estado_2= estado_2;
    return estadistica;
}

bool estadisticas_iguales(estadisticas_t estadistica_1, estadisticas_t estadistica_2){
    return estadistica_1.cantidad_CLT == estadistica_2.cantidad_CLT &&
           estadistica_1.cantidad_RBO == estadistica_2.cantidad_RBO &&
           estadistica_1.cantidad_KSC == estadistica_2.cantidad_KSC &&
           estadistica_1.cantidad_KDT == estadistica_2.cantidad_KDT &&
           estadistica_1.cantidad_estado_0 == estadistica_2.cantidad_estado_0 &&
           estadistica_1.cantidad_estado_1 == estadistica_2.cantidad_estado_1 &&
           estadistica_1.cantidad_estado_2 == estadistica_2.cantidad_estado_2;
}

void printEstadisticas(estadisticas_t estadistica){
    printf("CLT: %i \n", estadistica.cantidad_CLT);
    printf("RBO: %i \n", estadistica.cantidad_RBO);
    printf("KSC: %i \n", estadistica.cantidad_KSC);
    printf("KDT: %i \n", estadistica.cantidad_KDT);
    printf("estado_0: %i \n", estadistica.cantidad_estado_0);
    printf("estado_1: %i \n", estadistica.cantidad_estado_1);
    printf("estado_2: %i \n", estadistica.cantidad_estado_2);
}

/**
 * Evalúa los tests del ejercicio 3. Este ejercicio requiere implementar
 * `resolver_automaticamente`.
 *
 * En caso de que se quieran skipear los tests alcanza con asignarle `false`
 * a `EJERCICIO_3_HECHO`.
 */
TEST(ej3_calcular_estadisticas) {
    uint64_t failed_at_start = failed_tests;
    if (!EJERCICIO_3_HECHO) {
        printf("\n" SKIP "El ejercicio 3 no está hecho aún.\n");
        return;
    }

    // Test 1: Calcular sin filtrar por usuario
    caso_t casos_test1[6] = {
        crear_caso(1, 0, "CLT", 0),
        crear_caso(1, 0, "RBO", 1),
        crear_caso(2, 0, "CLT", 2),
        crear_caso(4, 0, "KDT", 0),
        crear_caso(3, 0, "KDT", 1),
        crear_caso(2, 0, "KDT", 0)
    };

    estadisticas_t* resultados1 = TEST_CALL_S(calcular_estadisticas, casos_test1, 6, 0);
    
    estadisticas_t salida_esperada1 = crear_estadistica(2,1,0,3,3,2,1);
    

    bool failed = false;
    
    if (!estadisticas_iguales(salida_esperada1, *resultados1)) {
        failed = true;
        printf("\n" FAIL "ej3_calcular_estadisticas_sin_filtro: No se contabilizó correctamente\n");
        printf("Se esperaba: \n");
        printEstadisticas(salida_esperada1);
        printf("Se obtuvo: \n");
        printEstadisticas(*resultados1);
    }

    //chequeo de integridad
    caso_t casos_test1_copia[6] = {
        crear_caso(1, 0, "CLT", 0),
        crear_caso(1, 0, "RBO", 1),
        crear_caso(2, 0, "CLT", 2),
        crear_caso(4, 0, "KDT", 0),
        crear_caso(3, 0, "KDT", 1),
        crear_caso(2, 0, "KDT", 0)
    };

    for (int i = 0; i < 6; i++){
        if (!casos_iguales(casos_test1[i], casos_test1_copia[i])) {
            failed = true;
            printf("\n" FAIL "ej3_calcular_estadisticas_sin_filtro: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf("\n" DONE "ej3_calcular_estadisticas_sin_filtro\n");
        successful_tests++;
    }

    liberar_casos(casos_test1, 6);
    liberar_casos(casos_test1_copia, 6);
    free(resultados1);

// Test 2: Calcular filtrando por usuario
    caso_t casos_test2[6] = {
        crear_caso(1, 0, "CLT", 0),
        crear_caso(1, 0, "RBO", 1),
        crear_caso(2, 0, "CLT", 2),
        crear_caso(4, 0, "KDT", 0),
        crear_caso(3, 0, "KDT", 1),
        crear_caso(2, 0, "KDT", 0)
    };

    estadisticas_t* resultados2 = TEST_CALL_S(calcular_estadisticas, casos_test2, 6, 2);
    
    estadisticas_t salida_esperada2 = crear_estadistica(1,0,0,1,1,0,1);
    

     failed = false;
    
    if (!estadisticas_iguales(salida_esperada2, *resultados2)) {
        failed = true;
        printf(FAIL "ej3_calcular_estadisticas_filtrando_por_usuario: No se contabilizó correctamente\n");
        printf("Se esperaba: \n");
        printEstadisticas(salida_esperada2);
        printf("Se obtuvo: \n");
        printEstadisticas(*resultados2);
    }

    //chequeo de integridad
    caso_t casos_test2_copia[6] = {
        crear_caso(1, 0, "CLT", 0),
        crear_caso(1, 0, "RBO", 1),
        crear_caso(2, 0, "CLT", 2),
        crear_caso(4, 0, "KDT", 0),
        crear_caso(3, 0, "KDT", 1),
        crear_caso(2, 0, "KDT", 0)
    };

    for (int i = 0; i < 6; i++){
        if (!casos_iguales(casos_test2[i], casos_test2_copia[i])) {
            failed = true;
            printf(FAIL "ej3_calcular_estadisticas_sin_filtro: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej3_calcular_estadisticas_filtrando_por_usuario\n");
        successful_tests++;
    }

    liberar_casos(casos_test2, 6);
    liberar_casos(casos_test2_copia, 6);
    free(resultados2);

// Test 3: Arreglo vacío
    caso_t casos_test3[0]; 

    estadisticas_t* resultados3 = TEST_CALL_S(calcular_estadisticas, casos_test3, 0, 2);
    
    estadisticas_t salida_esperada3 = crear_estadistica(0,0,0,0,0,0,0);
    failed = false;
    
    
    if (!estadisticas_iguales(salida_esperada3, *resultados3)) {
        failed = true;
        printf(FAIL "ej3_calcular_estadisticas_vacio: No se contabilizó correctamente\n");
        printf("Se esperaba: \n");
        printEstadisticas(salida_esperada3);
        printf("Se obtuvo: \n");
        printEstadisticas(*resultados3);
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej3_calcular_estadisticas_vacio\n");
        successful_tests++;
    }

    free(resultados3);

// Test 4: Casos con valores extremos de ID
    caso_t casos_test4[4] = {
        crear_caso(0, 0, "CLT", 1),           
        crear_caso(4294967255, 1, "RBO", 2),  
        crear_caso(1, 2, "KSC", 0),           
        crear_caso(999999255, 0, "KDT", 2)    
    };

    estadisticas_t* resultados4 = TEST_CALL_S(calcular_estadisticas, casos_test4, 4, 999999255);
    
    estadisticas_t salida_esperada4 = crear_estadistica(0,0,0,1,0,0,1);
    failed = false;
    
    if (!estadisticas_iguales(salida_esperada4, *resultados4)) {
        failed = true;
        printf(FAIL "ej3_calcular_estadisticas_valores_extremos: No se contabilizó correctamente\n");
        printf("Se esperaba: \n");
        printEstadisticas(salida_esperada4);
        printf("Se obtuvo: \n");
        printEstadisticas(*resultados4);
    }


    //chequeo de integridad
    caso_t casos_test4_copia[4] = {
        crear_caso(0, 0, "CLT", 1),           
        crear_caso(4294967255, 1, "RBO", 2),  
        crear_caso(1, 2, "KSC", 0),           
        crear_caso(999999255, 0, "KDT", 2)    
    };

    for (int i = 0; i < 4; i++){
        if (!casos_iguales(casos_test4[i], casos_test4_copia[i])) {
            failed = true;
            printf(FAIL "ej3_calcular_estadisticas_valores_extremos: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej3_calcular_estadisticas_valores_extremos\n");
        successful_tests++;
    }

    free(resultados4);
    liberar_casos(casos_test4, 4);
    liberar_casos(casos_test4_copia, 4);


    if (failed_at_start < failed_tests) {
        printf(FAIL "El ejercicio 3 tuvo tests que fallaron.\n");
    }
}


/**
 * Corre los tests de este ejercicio.
 *
 * La variable `EJERCICIO_3_HECHO` controla qué testsuites se van a correr. 
 * Asignarle `false` indica que el ejercicio no está implementado y por lo 
 * tanto no se ejecutarán sus tests.
 *
 * Si algún test falla el programa va a terminar con un código de error.
 */
int main(int argc, char* argv[]) {
    ej3_calcular_estadisticas();
    

    printf(
        "\nSe corrieron %lu tests. %lu corrieron exitosamente. %lu fallaron.\n",
        failed_tests + successful_tests, successful_tests, failed_tests
    );

    if (failed_tests || !EJERCICIO_3_HECHO) {
        return 1;
    } else {
        return 0;
    }
}