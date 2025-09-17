#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ej1.h"

/**
 * Marca el ejercicio 1A como hecho (`true`) o pendiente (`false`).
 *
 * Funciones a implementar:
 *   - es_indice_ordenado
 */
bool EJERCICIO_1A_HECHO = true;

/**
 * Marca el ejercicio 1B como hecho (`true`) o pendiente (`false`).
 *
 * Funciones a implementar:
 *   - contarCombustibleAsignado
 */
bool EJERCICIO_1B_HECHO = true;

/**
 * Marca el ejercicio 1B como hecho (`true`) o pendiente (`false`).
 *
 * Funciones a implementar:
 *   - modificarUnidad
 */
bool EJERCICIO_1C_HECHO = true;

/**
 * OPCIONAL: implementar en C
 */
void optimizar(mapa_t mapa, attackunit_t* compartida, uint32_t (*fun_hash)(attackunit_t*)) {
    uint32_t hash_compartida = fun_hash(compartida);

    for (int i = 0; i < 255; i++) {
        for (int j = 0; j < 255; j++) {
            attackunit_t* actual = mapa[i][j];
            if(actual == NULL || compartida == actual){
                continue;
            }
            uint32_t hash_actual = fun_hash(actual);
            if(hash_actual == hash_compartida){
                compartida->references++;
                actual->references--;
                mapa[i][j] = compartida;
            }
            if(actual->references == 0){
                free(actual);
            }
            
        }
    }
}


/**
 * OPCIONAL: implementar en C
 */
uint32_t contarCombustibleAsignado(mapa_t mapa, uint16_t (*fun_combustible)(char*)) {
    uint32_t total_combustible_utilizado = 0;
    for (uint64_t i = 0; i < 255; i++) {
        for (uint64_t j = 0; j < 255; j++) {
            attackunit_t* actual = mapa[i][j];
            if (actual == NULL) {
                continue;
            }
            uint32_t combustible_base = (uint32_t) fun_combustible(actual->clase);
            uint32_t combustible_utilizado = actual->combustible - combustible_base;
            total_combustible_utilizado += combustible_utilizado;
        }
    }
    return total_combustible_utilizado;
}


/**
 * OPCIONAL: implementar en C
 */
void modificarUnidad(mapa_t mapa, uint8_t x, uint8_t y, void (*fun_modificar)(attackunit_t*)) {
    attackunit_t* unidad = mapa[x][y];
    if (unidad == NULL) return;

    if (unidad->references > 1) { //fue optimizada?
        // Hacemos una copia
        attackunit_t* nueva = malloc(sizeof(attackunit_t));
        *nueva = *unidad;                 // copiamos contenido (struct entera)
        unidad->references--;            // quitamos una referencia a la compartida
        nueva->references = 1;           // la nueva es única
        mapa[x][y] = nueva;              // actualizamos el puntero del mapa
        unidad = nueva;                  // trabajamos sobre la nueva
    }

    fun_modificar(unidad);  // aplicamos la función
}
