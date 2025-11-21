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
bool EJERCICIO_2_HECHO = true;  // resolver_automaticamente


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
bool casos_iguales_excepto_estado(caso_t caso1, caso_t caso2){
    return strcmp(caso1.categoria, caso2.categoria) == 0 &&
           //caso1.estado == caso2.estado &&
           caso1.usuario->id == caso2.usuario->id &&
           caso1.usuario->nivel == caso2.usuario->nivel;

}

/*	Funcion cierra casos*/
uint16_t id_usuario_par_impar(caso_t* caso) {
    // Si el ID del usuario es par devuelve 0, si no devuelve 1
    return (caso->usuario->id % 2 == 0) ? 0 : 1;
}

/**
 * Función auxiliar para verificar que los casos fueron procesados correctamente
 */
bool verificar_casos_procesados(funcionCierraCasos_t* funcion, caso_t* casos, int largo, caso_t* casos_a_revisar, int largo_revisar) {
    // Verificar que los casos de nivel 0 van a revisión
    // Verificar que los casos de nivel 1 y 2 con resultado 0 y categoría CLT/RBO se cierran desfavorablemente
    // Verificar que los casos de nivel 1 y 2 con resultado 1 se cierran favorablemente
    // Verificar que los casos de nivel 1 y 2 con resultado 0 y categoría diferente van a revisión
    
    int casos_en_revision = 0;
    
    for (int i = 0; i < largo; i++) {
        uint32_t nivel = casos[i].usuario->nivel;
        uint16_t resultado = funcion(&casos[i]);
        
        if (nivel == 0) {
            // Casos de nivel 0 siempre van a revisión
            casos_en_revision++;
        } else if (nivel == 1 || nivel == 2) {
            if (resultado == 1) {
                // Se cierra favorablemente (estado = 1)
                if (casos[i].estado != 1) {
                    return false;
                }
            } else if (resultado == 0) {
                // Verificar categoría
                if (strcmp(casos[i].categoria, "CLT") == 0 || strcmp(casos[i].categoria, "RBO") == 0) {
                    // Se cierra desfavorablemente (estado = 2)
                    if (casos[i].estado != 2) {
                        return false;
                    }
                } else {
                    // Va a revisión
                    casos_en_revision++;
                }
            }
        }
    }
    
    return casos_en_revision == largo_revisar;
}

/**
 * Función auxiliar para contar casos que van a revisión
 */
int contar_casos_a_revision(funcionCierraCasos_t* funcion,caso_t* casos, int largo) {
    int contador = 0;
    
    for (int i = 0; i < largo; i++) {
        uint32_t nivel = casos[i].usuario->nivel;
        uint16_t resultado = funcion(&casos[i]);
        
        if (nivel == 0) {
            // Casos de nivel 0 siempre van a revisión
            contador++;
        } else if (nivel == 1 || nivel == 2) {
            if (resultado == 0) {
                // Verificar categoría
                if (strcmp(casos[i].categoria, "CLT") != 0 && strcmp(casos[i].categoria, "RBO") != 0) {
                    // Va a revisión
                    contador++;
                }
            }
        }
    }
    
    return contador;
}

/**
 * Evalúa los tests del ejercicio 2. Este ejercicio requiere implementar
 * `resolver_automaticamente`.
 *
 * En caso de que se quieran skipear los tests alcanza con asignarle `false`
 * a `EJERCICIO_2_HECHO`.
 */
TEST(ej2_resolver_automaticamente) {
    funcionCierraCasos_t* funcionCierraCasos = id_usuario_par_impar;

    uint64_t failed_at_start = failed_tests;
    if (!EJERCICIO_2_HECHO) {
        printf("\n" SKIP "El ejercicio 2 no está hecho aún.\n");
        return;
    }

    // Test 1: Casos de nivel 0 (siempre van a revisión)
    caso_t casos_test1[2] = {
        crear_caso(1, 0, "A1", 1),
        crear_caso(2, 0, "B2", 2)
    };

    caso_t casos_a_revisar1[2];
    TEST_CALL_V(resolver_automaticamente, funcionCierraCasos, casos_test1, casos_a_revisar1, 2);
    
    bool failed = false;
    
    // Verificar que los casos de nivel 0 mantienen su estado original
    if (casos_test1[0].estado != 1 || casos_test1[1].estado != 2) {
        failed = true;
        printf("\n" FAIL "ej2_resolver_automaticamente_nivel_0: casos de nivel 0 no mantuvieron su estado\n");
    }

    //chequeo de integridad
    caso_t casos_test1_copia[2] = {
        crear_caso(1, 0, "A1", 1),
        crear_caso(2, 0, "B2", 2)
    };

    for (int i = 0; i < 2; i++){
        if (!casos_iguales_excepto_estado(casos_test1[i], casos_test1_copia[i])) {
            failed = true;
            printf("\n" FAIL "ej2_resolver_automaticamente_nivel_0: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf("\n" DONE "ej2_resolver_automaticamente_nivel_0\n");
        successful_tests++;
    }

    liberar_casos(casos_test1, 2);
    liberar_casos(casos_test1_copia, 2);

    // Test 2: Casos de nivel 1 y 2 con resultado 1 (cierre favorable)
    caso_t casos_test2[3] = {
        crear_caso(3, 1, "C3", 3),  // ID impar -> resultado 1
        crear_caso(5, 2, "D5", 5),  // ID impar -> resultado 1
        crear_caso(7, 1, "E7", 7)   // ID impar -> resultado 1
    };

    caso_t casos_a_revisar2[3];
    TEST_CALL_V(resolver_automaticamente, funcionCierraCasos, casos_test2, casos_a_revisar2, 3);
    
    failed = false;
    
    // Verificar que los casos con resultado 1 se cerraron favorablemente (estado = 1)
    if (casos_test2[0].estado != 1 || casos_test2[1].estado != 1 || casos_test2[2].estado != 1) {
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_cierre_favorable: casos no se cerraron favorablemente\n");
    }

    //chequeo de integridad
    caso_t casos_test2_copia[3] = {
        crear_caso(3, 1, "C3", 3),  // ID impar -> resultado 1
        crear_caso(5, 2, "D5", 5),  // ID impar -> resultado 1
        crear_caso(7, 1, "E7", 7)   // ID impar -> resultado 1
    };

    for (int i = 0; i < 3; i++){
        if (!casos_iguales_excepto_estado(casos_test2[i], casos_test2_copia[i])) {
            failed = true;
            printf(FAIL "ej2_resolver_automaticamente_cierre_favorable: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej2_resolver_automaticamente_cierre_favorable\n");
        successful_tests++;
    }

    liberar_casos(casos_test2, 3);
    liberar_casos(casos_test2_copia, 3);

    // Test 3: Casos de nivel 1 y 2 con resultado 0 y categoría CLT/RBO (cierre desfavorable)
    caso_t casos_test3[4] = {
        crear_caso(2, 1, "CLT", 2),  // ID par -> resultado 0, categoría CLT
        crear_caso(4, 2, "RBO", 4),  // ID par -> resultado 0, categoría RBO
        crear_caso(6, 1, "CLT", 6),  // ID par -> resultado 0, categoría CLT
        crear_caso(8, 2, "RBO", 8)   // ID par -> resultado 0, categoría RBO
    };

    caso_t casos_a_revisar3[4];
    TEST_CALL_V(resolver_automaticamente, funcionCierraCasos, casos_test3, casos_a_revisar3, 4);
    
    failed = false;
    
    // Verificar que los casos con resultado 0 y categoría CLT/RBO se cerraron desfavorablemente (estado = 2)
    if (casos_test3[0].estado != 2 || casos_test3[1].estado != 2 || 
        casos_test3[2].estado != 2 || casos_test3[3].estado != 2) {
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_cierre_desfavorable: casos no se cerraron desfavorablemente\n");
    }

    //chequeo de integridad
    caso_t casos_test3_copia[4] = {
        crear_caso(2, 1, "CLT", 2),  // ID par -> resultado 0, categoría CLT
        crear_caso(4, 2, "RBO", 4),  // ID par -> resultado 0, categoría RBO
        crear_caso(6, 1, "CLT", 6),  // ID par -> resultado 0, categoría CLT
        crear_caso(8, 2, "RBO", 8)   // ID par -> resultado 0, categoría RBO
    };

    for (int i = 0; i < 4; i++){
        if (!casos_iguales_excepto_estado(casos_test3[i], casos_test3_copia[i])) {
            failed = true;
            printf(FAIL "ej2_resolver_automaticamente_cierre_desfavorable: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej2_resolver_automaticamente_cierre_desfavorable\n");
        successful_tests++;
    }

    liberar_casos(casos_test3, 4);
    liberar_casos(casos_test3_copia, 4);

    // Test 4: Casos de nivel 1 y 2 con resultado 0 y categoría diferente (van a revisión)
    caso_t casos_test4[3] = {
        crear_caso(2, 1, "ABC", 2),  // ID par -> resultado 0, categoría diferente
        crear_caso(4, 2, "XYZ", 4),  // ID par -> resultado 0, categoría diferente
        crear_caso(6, 1, "DEF", 6)   // ID par -> resultado 0, categoría diferente
    };

    caso_t casos_a_revisar4[3];
    TEST_CALL_V(resolver_automaticamente, funcionCierraCasos, casos_test4, casos_a_revisar4, 3);
    
    failed = false;
    
    // Verificar que los casos con resultado 0 y categoría diferente mantienen su estado original
    if (casos_test4[0].estado != 2 || casos_test4[1].estado != 4 || casos_test4[2].estado != 6) {
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_van_a_revision: casos no mantuvieron su estado\n");
    }

    //chequeo de integridad
    caso_t casos_test4_copia[3] = {
        crear_caso(2, 1, "ABC", 2),  // ID par -> resultado 0, categoría diferente
        crear_caso(4, 2, "XYZ", 4),  // ID par -> resultado 0, categoría diferente
        crear_caso(6, 1, "DEF", 6)   // ID par -> resultado 0, categoría diferente
    };

    for (int i = 0; i < 3; i++){
        if (!casos_iguales_excepto_estado(casos_test4[i], casos_test4_copia[i])) {
            failed = true;
            printf(FAIL "ej2_resolver_automaticamente_van_a_revision: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej2_resolver_automaticamente_van_a_revision\n");
        successful_tests++;
    }

    liberar_casos(casos_test4, 3);
    liberar_casos(casos_test4_copia, 3);

    // Test 5: Caso mixto con todos los tipos
    caso_t casos_test5[6] = {
        crear_caso(1, 0, "A1", 1),   // Nivel 0 -> va a revisión
        crear_caso(3, 1, "B3", 3),   // ID impar -> cierre favorable
        crear_caso(2, 2, "CLT", 2),  // ID par, CLT -> cierre desfavorable
        crear_caso(4, 1, "RBO", 4),  // ID par, RBO -> cierre desfavorable
        crear_caso(6, 2, "XYZ", 6),  // ID par, categoría diferente -> va a revisión
        crear_caso(5, 1, "C5", 5)    // ID impar -> cierre favorable
    };

    caso_t casos_a_revisar5[6];
    TEST_CALL_V(resolver_automaticamente, funcionCierraCasos, casos_test5, casos_a_revisar5, 6);
    
    failed = false;
    
    // Verificar cada caso según su tipo
    if (casos_test5[0].estado != 1) {  // Nivel 0 mantiene estado
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_mixto: caso nivel 0 no mantuvo estado\n");
    }
    if (casos_test5[1].estado != 1) {  // Cierre favorable
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_mixto: caso cierre favorable incorrecto\n");
    }
    if (casos_test5[2].estado != 2) {  // Cierre desfavorable CLT
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_mixto: caso cierre desfavorable CLT incorrecto\n");
    }
    if (casos_test5[3].estado != 2) {  // Cierre desfavorable RBO
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_mixto: caso cierre desfavorable RBO incorrecto\n");
    }
    if (casos_test5[4].estado != 6) {  // Va a revisión
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_mixto: caso va a revisión no mantuvo estado\n");
    }
    if (casos_test5[5].estado != 1) {  // Cierre favorable
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_mixto: caso cierre favorable incorrecto\n");
    }

        //chequeo de integridad
    caso_t casos_test5_copia[6] = {
        crear_caso(1, 0, "A1", 1),   // Nivel 0 -> va a revisión
        crear_caso(3, 1, "B3", 3),   // ID impar -> cierre favorable
        crear_caso(2, 2, "CLT", 2),  // ID par, CLT -> cierre desfavorable
        crear_caso(4, 1, "RBO", 4),  // ID par, RBO -> cierre desfavorable
        crear_caso(6, 2, "XYZ", 6),  // ID par, categoría diferente -> va a revisión
        crear_caso(5, 1, "C5", 5)    // ID impar -> cierre favorable
    };

    for (int i = 0; i < 6; i++){
        if (!casos_iguales_excepto_estado(casos_test5[i], casos_test5_copia[i])) {
            failed = true;
            printf(FAIL "ej2_resolver_automaticamente_mixto: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej2_resolver_automaticamente_mixto\n");
        successful_tests++;
    }

    liberar_casos(casos_test5, 6);
    liberar_casos(casos_test5_copia, 6);

    // Test 6: Arreglo vacío
    TEST_CALL_V(resolver_automaticamente, funcionCierraCasos, NULL, NULL, 0);
    printf(DONE "ej2_resolver_automaticamente_vacio\n");
    successful_tests++;

    // Test 7: Casos con valores extremos de ID
    caso_t casos_test7[4] = {
        crear_caso(0, 1, "CLT", 1),           // ID mínimo (par) -> cierre desfavorable
        crear_caso(4294967295, 2, "RBO", 2),  // ID máximo (impar) -> cierre favorable
        crear_caso(1, 1, "ABC", 3),           // ID pequeño (impar) -> cierre favorable
        crear_caso(999999998, 2, "XYZ", 4)    // ID grande (par) -> va a revisión
    };

    caso_t casos_a_revisar7[4];
    TEST_CALL_V(resolver_automaticamente, funcionCierraCasos, casos_test7, casos_a_revisar7, 4);
    
    failed = false;
    
    // Verificar casos con valores extremos
    if (casos_test7[0].estado != 2) {  // ID 0 (par) con CLT -> cierre desfavorable
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_valores_extremos: ID 0 incorrecto\n");
    }
    if (casos_test7[1].estado != 1) {  // ID máximo (impar) -> cierre favorable
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_valores_extremos: ID máximo incorrecto\n");
    }
    if (casos_test7[2].estado != 1) {  // ID 1 (impar) -> cierre favorable
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_valores_extremos: ID 1 incorrecto\n");
    }
    if (casos_test7[3].estado != 4) {  // ID grande (par) con categoría diferente -> mantiene estado
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_valores_extremos: ID grande incorrecto\n");
    }

    //chequeo de integridad
    caso_t casos_test7_copia[4] = {
        crear_caso(0, 1, "CLT", 1),           // ID mínimo (par) -> cierre desfavorable
        crear_caso(4294967295, 2, "RBO", 2),  // ID máximo (impar) -> cierre desfavorable
        crear_caso(1, 1, "ABC", 3),           // ID pequeño (impar) -> cierre favorable
        crear_caso(999999998, 2, "XYZ", 4)    // ID grande (par) -> va a revisión
    };

    for (int i = 0; i < 4; i++){
        if (!casos_iguales_excepto_estado(casos_test7[i], casos_test7_copia[i])) {
            failed = true;
            printf(FAIL "ej2_resolver_automaticamente_valores_extremos: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej2_resolver_automaticamente_valores_extremos\n");
        successful_tests++;
    }

    liberar_casos(casos_test7, 4);
    liberar_casos(casos_test7_copia, 4);

    // Test 8: Casos con estados extremos
    caso_t casos_test8[3] = {
        crear_caso(2, 1, "CLT", 0),           // Estado mínimo
        crear_caso(6, 2, "RBO", 65535),       // Estado máximo (UINT16_MAX)
        crear_caso(4, 1, "ABC", 32767)        // Estado medio
    };

    caso_t casos_a_revisar8[3];
    TEST_CALL_V(resolver_automaticamente, funcionCierraCasos, casos_test8, casos_a_revisar8, 3);
    
    failed = false;
    
    // Verificar que los estados se actualizan correctamente
    if (casos_test8[0].estado != 2) {  // Debe cambiar a cierre desfavorable
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_estados_extremos: estado mínimo incorrecto\n");
    }
    if (casos_test8[1].estado != 2) {  // Debe cambiar a cierre desfavorable
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_estados_extremos: estado máximo incorrecto\n");
    }
    if (casos_test8[2].estado != 32767) {  // Debe mantener estado (va a revisión)
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_estados_extremos: estado medio incorrecto\n");
    }

    //chequeo de integridad
    caso_t casos_test8_copia[3] = {
        crear_caso(2, 1, "CLT", 0),           // Estado mínimo
        crear_caso(6, 2, "RBO", 65535),       // Estado máximo (UINT16_MAX)
        crear_caso(4, 1, "ABC", 32767)        // Estado medio
    };

    for (int i = 0; i < 3; i++){
        if (!casos_iguales_excepto_estado(casos_test8[i], casos_test8_copia[i])) {
            failed = true;
            printf(FAIL "ej2_resolver_automaticamente_estados_extremos: Se modificaron datos de entrada\n");
            break;
        }        
    }


    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej2_resolver_automaticamente_estados_extremos\n");
        successful_tests++;
    }

    liberar_casos(casos_test8, 3);
    liberar_casos(casos_test8_copia, 3);

    // Test 9: Casos con categorías edge cases
    caso_t casos_test9[6] = {
        crear_caso(2, 1, "CLT", 1),   // CLT exacto
        crear_caso(2, 1, "RBO", 2),   // RBO exacto
        crear_caso(2, 1, "clt", 3),   // CLT en minúsculas (no debe coincidir)
        crear_caso(2, 1, "rbo", 4),   // RBO en minúsculas (no debe coincidir)
        crear_caso(2, 1, "CLTX", 5),  // CLT con caracteres extra
        crear_caso(2, 1, "RBOY", 6)   // RBO con caracteres extra
    };

    caso_t casos_a_revisar9[6];
    TEST_CALL_V(resolver_automaticamente, funcionCierraCasos, casos_test9, casos_a_revisar9, 6);
    
    failed = false;
    
    // Verificar que solo CLT y RBO exactos se cierran desfavorablemente
    if (casos_test9[0].estado != 2) {  // CLT exacto -> cierre desfavorable
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_categorias_edge: CLT exacto incorrecto\n");
    }
    if (casos_test9[1].estado != 2) {  // RBO exacto -> cierre desfavorable
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_categorias_edge: RBO exacto incorrecto\n");
    }
    if (casos_test9[2].estado != 3) {  // clt minúsculas -> mantiene estado
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_categorias_edge: clt minúsculas incorrecto\n");
    }
    if (casos_test9[3].estado != 4) {  // rbo minúsculas -> mantiene estado
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_categorias_edge: rbo minúsculas incorrecto\n");
    }
    if (casos_test9[4].estado != 5) {  // CLTX -> mantiene estado
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_categorias_edge: CLTX incorrecto\n");
    }
    if (casos_test9[5].estado != 6) {  // RBOY -> mantiene estado
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_categorias_edge: RBOY incorrecto\n");
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej2_resolver_automaticamente_categorias_edge\n");
        successful_tests++;
    }

    liberar_casos(casos_test9, 6);

    // Test 10: Casos con distribución desbalanceada
    caso_t casos_test10[10] = {
        crear_caso(1, 0, "A1", 1),   // 1 caso nivel 0
        crear_caso(3, 1, "B3", 3),   // 8 casos nivel 1 (todos impares -> cierre favorable)
        crear_caso(5, 1, "B5", 5),
        crear_caso(7, 1, "B7", 7),
        crear_caso(9, 1, "B9", 9),
        crear_caso(11, 1, "B11", 11),
        crear_caso(13, 1, "B13", 13),
        crear_caso(15, 1, "B15", 15),
        crear_caso(17, 1, "B17", 17),
        crear_caso(2, 2, "CLT", 2)   // 1 caso nivel 2 (par con CLT -> cierre desfavorable)
    };

    caso_t casos_a_revisar10[10];
    TEST_CALL_V(resolver_automaticamente, funcionCierraCasos, casos_test10, casos_a_revisar10, 10);
    
    failed = false;
    
    // Verificar que todos los casos se procesaron correctamente
    if (casos_test10[0].estado != 1) {  // Nivel 0 mantiene estado
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_distribucion_desbalanceada: nivel 0 incorrecto\n");
    }
    
    // Verificar que todos los casos de nivel 1 (impares) se cerraron favorablemente
    for (int i = 1; i < 9; i++) {
        if (casos_test10[i].estado != 1) {
            failed = true;
            printf(FAIL "ej2_resolver_automaticamente_distribucion_desbalanceada: caso %d incorrecto\n", i);
            break;
        }
    }
    
    if (casos_test10[9].estado != 2) {  // Nivel 2 (par con CLT) -> cierre desfavorable
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_distribucion_desbalanceada: último caso incorrecto\n");
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej2_resolver_automaticamente_distribucion_desbalanceada\n");
        successful_tests++;
    }

    liberar_casos(casos_test10, 10);

    // Test 11: Verificación de memoria (casos grandes)
    int num_casos = 100;
    caso_t* casos_test11 = malloc(num_casos * sizeof(caso_t));
    caso_t* casos_a_revisar11 = malloc(num_casos * sizeof(caso_t));
    
    for (int i = 0; i < num_casos; i++) {
        casos_test11[i] = crear_caso(i, i % 3, (i % 4 == 0) ? "CLT" : (i % 4 == 1) ? "RBO" : "ABC", i);
    }

    TEST_CALL_V(resolver_automaticamente, funcionCierraCasos, casos_test11, casos_a_revisar11, num_casos);
    
    failed = false;
    
    // Verificar que se procesaron correctamente
    int casos_esperados_revision = contar_casos_a_revision(funcionCierraCasos, casos_test11, num_casos);
    
    if (casos_esperados_revision < 0) {
        failed = true;
        printf(FAIL "ej2_resolver_automaticamente_memoria_grande: error en conteo de casos\n");
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej2_resolver_automaticamente_memoria_grande\n");
        successful_tests++;
    }

    liberar_casos(casos_test11, num_casos);
    free(casos_test11);
    free(casos_a_revisar11);

    if (failed_at_start < failed_tests) {
        printf(FAIL "El ejercicio 2 tuvo tests que fallaron.\n");
    }
}

/**
 * Test adicional para verificar la función id_usuario_par
 */
void ej2_funcion_cierra_casos() {
    caso_t caso_par = crear_caso(2, 0, "A1", 1);
    caso_t caso_impar = crear_caso(3, 1, "B2", 2);
    
    bool failed = false;
    
    if (id_usuario_par_impar(&caso_par) != 0) {
        failed = true;
        printf(FAIL "ej2_funcion_cierra_casos: ID par debería devolver 0\n");
    }
    
    if (id_usuario_par_impar(&caso_impar) != 1) {
        failed = true;
        printf(FAIL "ej2_funcion_cierra_casos: ID impar debería devolver 1\n");
    }
    
    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej2_funcion_cierra_casos\n");
        successful_tests++;
    }
    
    free(caso_par.usuario);
    free(caso_impar.usuario);
}

/**
 * Corre los tests de este ejercicio.
 *
 * La variable `EJERCICIO_2_HECHO` controla qué testsuites se van a correr. 
 * Asignarle `false` indica que el ejercicio no está implementado y por lo 
 * tanto no se ejecutarán sus tests.
 *
 * Si algún test falla el programa va a terminar con un código de error.
 */
int main(int argc, char* argv[]) {
    ej2_resolver_automaticamente();
    //ej2_funcion_cierra_casos();

    printf(
        "\nSe corrieron %lu tests. %lu corrieron exitosamente. %lu fallaron.\n",
        failed_tests + successful_tests, successful_tests, failed_tests
    );

    if (failed_tests || !EJERCICIO_2_HECHO) {
        return 1;
    } else {
        return 0;
    }
}