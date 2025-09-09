#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ej2.h"

/**
 * Marca el ejercicio 1A como hecho (`true`) o pendiente (`false`).
 *
 * Funciones a implementar:
 *   - es_indice_ordenado
 */
bool EJERCICIO_2A_HECHO = true;

/**
 * Marca el ejercicio 1B como hecho (`true`) o pendiente (`false`).
 *
 * Funciones a implementar:
 *   - contarCombustibleAsignado
 */
bool EJERCICIO_2B_HECHO = true;

/**
 * Marca el ejercicio 1B como hecho (`true`) o pendiente (`false`).
 *
 * Funciones a implementar:
 *   - modificarUnidad
 */
bool EJERCICIO_2C_HECHO = true;

/**
 * OPCIONAL: implementar en C
 */
void optimizar(mapa_t mapa, attackunit_t *compartida,
               uint32_t (*fun_hash)(attackunit_t *)) {

  if (mapa == NULL || compartida == NULL || fun_hash == NULL) {
    return;
  }

  for (int i = 0; i < 255; i++) {
    for (int j = 0; j < 255; j++) {
      if (mapa[i][j] != NULL &&
          (*fun_hash)(mapa[i][j]) == (*fun_hash)(compartida) &&
          compartida != mapa[i][j]) {

        mapa[i][j]->references--;
        if (mapa[i][j]->references == 0) {
          free(mapa[i][j]);
        }

        mapa[i][j] = compartida;
        compartida->references++;
      }
    }
  }
}

/**
 * OPCIONAL: implementar en C
 */
uint32_t contarCombustibleAsignado(mapa_t mapa,
                                   uint16_t (*fun_combustible)(char *)) {
  if (mapa == NULL || fun_combustible == NULL) {
    return -1;
  }

  uint32_t res = 0;

  for (int i = 0; i < 255; i++) {
    for (int j = 0; j < 255; j++) {
      if (mapa[i][j] != NULL) {
        res += mapa[i][j]->combustible;
        res -= (*fun_combustible)((char *)mapa[i][j]->clase);
      }
    }
  }
  return res;
}

/**
 * OPCIONAL: implementar en C
 */
void modificarUnidad(mapa_t mapa, uint8_t x, uint8_t y,
                     void (*fun_modificar)(attackunit_t *)) {
  if ((x >= 255 || y >= 255) || mapa == NULL || fun_modificar == NULL) {
    return;
  }

  attackunit_t *modifer = mapa[x][y];
  if (modifer == NULL) {
    return; // si no hay nada, no hago nada
  }

  // caso en el que esta optimizado, creamos un nueva unidad con el mismo
  // nombre, con una sola referencia y con el mismo combustible
  if (modifer->references > 1) {
    attackunit_t *new = malloc(sizeof(attackunit_t));
    modifer->references--;
    if (new == NULL) {
      return;
    }

    strncpy(new->clase, modifer->clase, 11);
    new->references = 1;
    new->combustible = modifer->combustible;
    (*fun_modificar)(new);
    mapa[x][y] = new;
  } else {
    // caso contrario solo modificamos la ya existente
    (*fun_modificar)(modifer);
  }
}
