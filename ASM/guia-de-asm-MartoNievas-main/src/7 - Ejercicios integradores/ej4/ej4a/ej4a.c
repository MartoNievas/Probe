#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ej4a.h"

/**
 * Marca el ejercicio 1A como hecho (`true`) o pendiente (`false`).
 *
 * Funciones a implementar:
 *   - init_fantastruco_dir
 */
bool EJERCICIO_1A_HECHO = true;

// OPCIONAL: implementar en C
void init_fantastruco_dir(fantastruco_t *card) {
  if (card == NULL)
    return;

  card->__dir = malloc(sizeof(directory_t) * 2);
  card->__dir_entries = 2;
  card->__dir[0] = create_dir_entry("sleep", &sleep);
  card->__dir[1] = create_dir_entry("wakeup", &wakeup);
}

/**
 * Marca el ejercicio 1A como hecho (`true`) o pendiente (`false`).
 *
 * Funciones a implementar:
 *   - summon_fantastruco
 */
bool EJERCICIO_1B_HECHO = true;

// OPCIONAL: implementar en Cj
fantastruco_t *summon_fantastruco() {

  fantastruco_t *res = malloc(sizeof(fantastruco_t));
  if (res == NULL) {
    return NULL;
  }

  init_fantastruco_dir(res);
  res->__archetype = NULL;
  res->face_up = 1;
  return res;
}
