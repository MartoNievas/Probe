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
 *   - indice_a_inventario
 */
bool EJERCICIO_1B_HECHO = true;

/**
 * OPCIONAL: implementar en C
 */
// comparador_t es un puntero a funcion la cual toma como parametro dos item_t
// Lo que entiendo por este ejercicio es que tengo que comprar si todos los
// elementos estan ordenador segun el comparado, entonces usando transitividad
// si inv[0] y inv[1], inv[1] y inv[2] estan ordenados entonces
//  inv[0] e inv[2] estan ordenados
bool es_indice_ordenado(item_t **inventario, uint16_t *indice, uint16_t tamanio,
                        comparador_t comparador) {
  if (inventario == NULL || indice == NULL)
    return false;
  for (int i = 0; i < tamanio - 1; i++) {
    if (!(*comparador)(inventario[indice[i]], inventario[indice[i + 1]])) {
      return false;
    }
  }
  return true;
}

/**
 * OPCIONAL: implementar en C
 */
item_t **indice_a_inventario(item_t **inventario, uint16_t *indice,
                             uint16_t tamanio) {
  // ¿Cuánta memoria hay que pedir para el resultado?
  // si no me equivoco malloc(sizeof(item_t)*tamanio)
  if (inventario == NULL || indice == NULL)
    return NULL;

  item_t **resultado = malloc(sizeof(item_t) * tamanio);
  for (int i = 0; i < tamanio; i++) {
    item_t *curr = inventario[indice[i]];

    resultado[i] = curr;
  }

  return resultado;
}
