#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ejs.h"
#include "../tests-aux.h"

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
bool EJERCICIO_1_HECHO = true;  // segmentar_casos


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

/**
 * Función auxiliar para liberar memoria de una segmentación
 */
void liberar_segmentacion(segmentacion_t* seg) {
    if (seg) {
        if (seg->casos_nivel_0) free(seg->casos_nivel_0);
        if (seg->casos_nivel_1) free(seg->casos_nivel_1);
        if (seg->casos_nivel_2) free(seg->casos_nivel_2);
        free(seg);
    }
}

/**
 * Función auxiliar para verificar que una segmentación es correcta
 */
bool verificar_segmentacion(segmentacion_t* seg, int* esperados_nivel_0, int* esperados_nivel_1, int* esperados_nivel_2) {
    // Verificar que los punteros son correctos según los casos esperados
    bool correcto = true;
    
    // Si se esperan casos de nivel 0, el puntero no debe ser NULL
    if (*esperados_nivel_0 > 0 && seg->casos_nivel_0 == NULL) {
        correcto = false;
    }
    
    // Si no se esperan casos de nivel 0, el puntero debe ser NULL
    if (*esperados_nivel_0 == 0 && seg->casos_nivel_0 != NULL) {
        correcto = false;
    }
    
    // Si se esperan casos de nivel 1, el puntero no debe ser NULL
    if (*esperados_nivel_1 > 0 && seg->casos_nivel_1 == NULL) {
        correcto = false;
    }
    
    // Si no se esperan casos de nivel 1, el puntero debe ser NULL
    if (*esperados_nivel_1 == 0 && seg->casos_nivel_1 != NULL) {
        correcto = false;
    }
    
    // Si se esperan casos de nivel 2, el puntero no debe ser NULL
    if (*esperados_nivel_2 > 0 && seg->casos_nivel_2 == NULL) {
        correcto = false;
    }
    
    // Si no se esperan casos de nivel 2, el puntero debe ser NULL
    if (*esperados_nivel_2 == 0 && seg->casos_nivel_2 != NULL) {
        correcto = false;
    }
    
    return correcto;
}

/**
 * Función auxiliar para verificar que los casos están correctamente distribuidos
 */
bool verificar_distribucion_casos(segmentacion_t* seg, caso_t* casos_originales, int largo) {
    // Contar casos en cada nivel
    int cont_0 = 0, cont_1 = 0, cont_2 = 0;
    
    // Contar en el arreglo original
    for (int i = 0; i < largo; i++) {
        uint32_t nivel = casos_originales[i].usuario->nivel;
        if (nivel == 0) cont_0++;
        else if (nivel == 1) cont_1++;
        else if (nivel == 2) cont_2++;
    }
    
    // Verificar que los punteros coinciden con los conteos
    if (cont_0 > 0 && seg->casos_nivel_0 == NULL) return false;
    if (cont_1 > 0 && seg->casos_nivel_1 == NULL) return false;
    if (cont_2 > 0 && seg->casos_nivel_2 == NULL) return false;
    
    return true;
}

/**
 * Evalúa los tests del ejercicio 1. Este ejercicio requiere implementar
 * `segmentar_casos`.
 *
 * En caso de que se quieran skipear los tests alcanza con asignarle `false`
 * a `EJERCICIO_1_HECHO`.
 */
TEST(ej1_segmentar_casos) {
    uint64_t failed_at_start = failed_tests;
    if (!EJERCICIO_1_HECHO) {
        printf("\n" SKIP "El ejercicio 1 no está hecho aún.\n");
        return;
    }

    // Test 1: Casos de todos los niveles
    caso_t casos_test1[6] = {
        crear_caso(1, 0, "A1", 1),
        crear_caso(2, 1, "B2", 2),
        crear_caso(3, 2, "C3", 3),
        crear_caso(4, 0, "A4", 4),
        crear_caso(5, 1, "B5", 5),
        crear_caso(6, 2, "C6", 6)
    };

    segmentacion_t* seg1 = TEST_CALL_S(segmentar_casos, casos_test1, 6);
    bool failed = false;
    
    if (!seg1) {
        failed = true;
        printf("\n" FAIL "ej1_segmentar_casos_todos_los_niveles: segmentar_casos devolvió NULL\n");
    } else {
        int esperados_0 = 2, esperados_1 = 2, esperados_2 = 2;
        failed = !verificar_segmentacion(seg1, &esperados_0, &esperados_1, &esperados_2);
        
        if (failed) {
            printf("\n" FAIL "ej1_segmentar_casos_todos_los_niveles: segmentación incorrecta\n");
        }
    }

    //chequeo de integridad
    caso_t casos_test1_copia[6] = {
        crear_caso(1, 0, "A1", 1),
        crear_caso(2, 1, "B2", 2),
        crear_caso(3, 2, "C3", 3),
        crear_caso(4, 0, "A4", 4),
        crear_caso(5, 1, "B5", 5),
        crear_caso(6, 2, "C6", 6)
    };

    for (int i = 0; i < 6; i++){
        if (!casos_iguales(casos_test1[i], casos_test1_copia[i])) {
            failed = true;
            printf("\n" FAIL "ej1_segmentar_casos_todos_los_niveles: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf("\n" DONE "ej1_segmentar_casos_todos_los_niveles\n");
        successful_tests++;
    }

    liberar_segmentacion(seg1);
    liberar_casos(casos_test1, 6);
    liberar_casos(casos_test1_copia, 6);

    // Test 2: Solo casos de nivel 0
    caso_t casos_test2[3] = {
        crear_caso(1, 0, "A1", 1),
        crear_caso(2, 0, "A2", 2),
        crear_caso(3, 0, "A3", 3)
    };

    segmentacion_t* seg2 = TEST_CALL_S(segmentar_casos, casos_test2, 3);
    failed = false;
    
    if (!seg2) {
        failed = true;
        printf(FAIL "ej1_segmentar_casos_nivel_0: segmentar_casos devolvió NULL\n");
    } else {
        int esperados_0 = 3, esperados_1 = 0, esperados_2 = 0;
        failed = !verificar_segmentacion(seg2, &esperados_0, &esperados_1, &esperados_2);
        
        if (failed) {
            printf(FAIL "ej1_segmentar_casos_nivel_0: segmentación incorrecta\n");
        }
    }

    //chequeo de integridad
    caso_t casos_test2_copia[3] = {
        crear_caso(1, 0, "A1", 1),
        crear_caso(2, 0, "A2", 2),
        crear_caso(3, 0, "A3", 3)
    };

    for (int i = 0; i < 3; i++){
        if (!casos_iguales(casos_test2[i], casos_test2_copia[i])) {
            failed = true;
            printf(FAIL "ej1_segmentar_casos_nivel_0: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej1_segmentar_casos_nivel_0\n");
        successful_tests++;
    }

    liberar_segmentacion(seg2);
    liberar_casos(casos_test2, 3);
    liberar_casos(casos_test2_copia, 3);

    // Test 3: Solo casos de nivel 1
    caso_t casos_test3[2] = {
        crear_caso(1, 1, "B1", 1),
        crear_caso(2, 1, "B2", 2)
    };

    segmentacion_t* seg3 = TEST_CALL_S(segmentar_casos, casos_test3, 2);
    failed = false;
    
    if (!seg3) {
        failed = true;
        printf(FAIL "ej1_segmentar_casos_nivel_1: segmentar_casos devolvió NULL\n");
    } else {
        int esperados_0 = 0, esperados_1 = 2, esperados_2 = 0;
        failed = !verificar_segmentacion(seg3, &esperados_0, &esperados_1, &esperados_2);
        
        if (failed) {
            printf(FAIL "ej1_segmentar_casos_nivel_1: segmentación incorrecta\n");
        }
    }

     //chequeo de integridad
    caso_t casos_test3_copia[2] = {
        crear_caso(1, 1, "B1", 1),
        crear_caso(2, 1, "B2", 2)
    };

    for (int i = 0; i < 2; i++){
        if (!casos_iguales(casos_test3[i], casos_test3_copia[i])) {
            failed = true;
            printf(FAIL "ej1_segmentar_casos_nivel_1: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej1_segmentar_casos_nivel_1\n");
        successful_tests++;
    }

    liberar_segmentacion(seg3);
    liberar_casos(casos_test3, 2);
    liberar_casos(casos_test3_copia, 2);

    // Test 4: Solo casos de nivel 2
    caso_t casos_test4[1] = {
        crear_caso(1, 2, "C1", 1)
    };

    segmentacion_t* seg4 = TEST_CALL_S(segmentar_casos, casos_test4, 1);
    failed = false;
    
    if (!seg4) {
        failed = true;
        printf(FAIL "ej1_segmentar_casos_nivel_2: segmentar_casos devolvió NULL\n");
    } else {
        int esperados_0 = 0, esperados_1 = 0, esperados_2 = 1;
        failed = !verificar_segmentacion(seg4, &esperados_0, &esperados_1, &esperados_2);
        
        if (failed) {
            printf(FAIL "ej1_segmentar_casos_nivel_2: segmentación incorrecta\n");
        }
    }

    //chequeo de integridad
    caso_t casos_test4_copia[1] = {
        crear_caso(1, 2, "C1", 1)
    };

    for (int i = 0; i < 1; i++){
        if (!casos_iguales(casos_test4[i], casos_test4_copia[i])) {
            failed = true;
            printf(FAIL "ej1_segmentar_casos_nivel_2: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej1_segmentar_casos_nivel_2\n");
        successful_tests++;
    }

    liberar_segmentacion(seg4);
    liberar_casos(casos_test4, 1);
    liberar_casos(casos_test4_copia, 1);

    // Test 5: Arreglo vacío
    caso_t casos_vacios[0]; 
    segmentacion_t* seg5 = TEST_CALL_S(segmentar_casos, casos_vacios, 0);
    failed = false;
    
    if (!seg5) {
        failed = true;
        printf(FAIL "ej1_segmentar_casos_vacio: segmentar_casos devolvió NULL\n");
    } else {
        int esperados_0 = 0, esperados_1 = 0, esperados_2 = 0;
        failed = !verificar_segmentacion(seg5, &esperados_0, &esperados_1, &esperados_2);
        
        if (failed) {
            printf(FAIL "ej1_segmentar_casos_vacio: segmentación incorrecta\n");
        }
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej1_segmentar_casos_vacio\n");
        successful_tests++;
    }

    liberar_segmentacion(seg5);

    // Test 6: Casos con valores extremos de ID
    caso_t casos_test6[4] = {
        crear_caso(0, 0, "A0", 1),           
        crear_caso(4294967295, 1, "B1", 2),  
        crear_caso(1, 2, "C2", 3),           
        crear_caso(999999999, 0, "A3", 4)    
    };

    segmentacion_t* seg6 = TEST_CALL_S(segmentar_casos, casos_test6, 4);
    failed = false;
    
    if (!seg6) {
        failed = true;
        printf(FAIL "ej1_segmentar_casos_valores_extremos: segmentar_casos devolvió NULL\n");
    } else {
        int esperados_0 = 2, esperados_1 = 1, esperados_2 = 1;
        failed = !verificar_segmentacion(seg6, &esperados_0, &esperados_1, &esperados_2);
        
        if (failed) {
            printf(FAIL "ej1_segmentar_casos_valores_extremos: segmentación incorrecta\n");
        }
    }

    //chequeo de integridad
    caso_t casos_test6_copia[4] = {
        crear_caso(0, 0, "A0", 1),           
        crear_caso(4294967295, 1, "B1", 2),  
        crear_caso(1, 2, "C2", 3),           
        crear_caso(999999999, 0, "A3", 4)   
    };

    for (int i = 0; i < 4; i++){
        if (!casos_iguales(casos_test6[i], casos_test6_copia[i])) {
            failed = true;
            printf(FAIL "ej1_segmentar_casos_valores_extremos: Se modificaron datos de entrada\n");
            break;
        }        
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej1_segmentar_casos_valores_extremos\n");
        successful_tests++;
    }

    liberar_segmentacion(seg6);
    liberar_casos(casos_test6, 4);
    liberar_casos(casos_test6_copia, 4);

    // Test 7: Casos con categorías especiales
    caso_t casos_test7[3] = {
        crear_caso(1, 0, "CLT", 1),  
        crear_caso(2, 1, "RBO", 2),  
        crear_caso(3, 2, "XYZ", 3)   
    };

    segmentacion_t* seg7 = TEST_CALL_S(segmentar_casos, casos_test7, 3);
    failed = false;
    
    if (!seg7) {
        failed = true;
        printf(FAIL "ej1_segmentar_casos_categorias_especiales: segmentar_casos devolvió NULL\n");
    } else {
        int esperados_0 = 1, esperados_1 = 1, esperados_2 = 1;
        failed = !verificar_segmentacion(seg7, &esperados_0, &esperados_1, &esperados_2);
        
        if (failed) {
            printf(FAIL "ej1_segmentar_casos_categorias_especiales: segmentación incorrecta\n");
        }
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej1_segmentar_casos_categorias_especiales\n");
        successful_tests++;
    }

    liberar_segmentacion(seg7);
    liberar_casos(casos_test7, 3);

    
    caso_t casos_test8[3] = {
        crear_caso(1, 0, "A1", 0),          
        crear_caso(2, 1, "B2", 65535),       
        crear_caso(3, 2, "C3", 32767)        
    };

    segmentacion_t* seg8 = TEST_CALL_S(segmentar_casos, casos_test8, 3);
    failed = false;
    
    if (!seg8) {
        failed = true;
        printf(FAIL "ej1_segmentar_casos_estados_extremos: segmentar_casos devolvió NULL\n");
    } else {
        int esperados_0 = 1, esperados_1 = 1, esperados_2 = 1;
        failed = !verificar_segmentacion(seg8, &esperados_0, &esperados_1, &esperados_2);
        
        if (failed) {
            printf(FAIL "ej1_segmentar_casos_estados_extremos: segmentación incorrecta\n");
        }
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej1_segmentar_casos_estados_extremos\n");
        successful_tests++;
    }

    liberar_segmentacion(seg8);
    liberar_casos(casos_test8, 3);

    caso_t casos_test9[10] = {
        crear_caso(1, 0, "A1", 1),   
        crear_caso(2, 1, "B2", 2),   
        crear_caso(3, 1, "B3", 3),
        crear_caso(4, 1, "B4", 4),
        crear_caso(5, 1, "B5", 5),
        crear_caso(6, 1, "B6", 6),
        crear_caso(7, 1, "B7", 7),
        crear_caso(8, 1, "B8", 8),
        crear_caso(9, 1, "B9", 9),
        crear_caso(10, 2, "C10", 10) 
    };

    segmentacion_t* seg9 = TEST_CALL_S(segmentar_casos, casos_test9, 10);
    failed = false;
    
    if (!seg9) {
        failed = true;
        printf(FAIL "ej1_segmentar_casos_distribucion_desbalanceada: segmentar_casos devolvió NULL\n");
    } else {
        int esperados_0 = 1, esperados_1 = 8, esperados_2 = 1;
        failed = !verificar_segmentacion(seg9, &esperados_0, &esperados_1, &esperados_2);
        
        if (failed) {
            printf(FAIL "ej1_segmentar_casos_distribucion_desbalanceada: segmentación incorrecta\n");
        }
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej1_segmentar_casos_distribucion_desbalanceada\n");
        successful_tests++;
    }

    liberar_segmentacion(seg9);
    liberar_casos(casos_test9, 10);

    // Test 10: Verificación de memoria (casos grandes)
    int num_casos = 100;
    caso_t* casos_test10 = malloc(num_casos * sizeof(caso_t));
    
    for (int i = 0; i < num_casos; i++) {
        casos_test10[i] = crear_caso(i, i % 3, "ABC", i);
    }

    segmentacion_t* seg10 = TEST_CALL_S(segmentar_casos, casos_test10, num_casos);
    failed = false;
    
    if (!seg10) {
        failed = true;
        printf(FAIL "ej1_segmentar_casos_memoria_grande: segmentar_casos devolvió NULL\n");
    } else {
        failed = !verificar_distribucion_casos(seg10, casos_test10, num_casos);
        
        if (failed) {
            printf(FAIL "ej1_segmentar_casos_memoria_grande: distribución incorrecta\n");
        }
    }

    if (failed) {
        failed_tests++;
    } else {
        printf(DONE "ej1_segmentar_casos_memoria_grande\n");
        successful_tests++;
    }

    liberar_segmentacion(seg10);
    liberar_casos(casos_test10, num_casos);
    free(casos_test10);

    if (failed_at_start < failed_tests) {
        printf(FAIL "El ejercicio 1 tuvo tests que fallaron.\n");
    }
}

/**
 * Corre los tests de este ejercicio.
 *
 * La variable `EJERCICIO_1_HECHO` controla qué testsuites se van a correr. 
 * Asignarle `false` indica que el ejercicio no está implementado y por lo 
 * tanto no se ejecutarán sus tests.
 *
 * Si algún test falla el programa va a terminar con un código de error.
 */
int main(int argc, char* argv[]) {
    ej1_segmentar_casos();

    printf(
        "\nSe corrieron %lu tests. %lu corrieron exitosamente. %lu fallaron.\n",
        failed_tests + successful_tests, successful_tests, failed_tests
    );

    if (failed_tests || !EJERCICIO_1_HECHO) {
        return 1;
    } else {
        return 0;
    }
}