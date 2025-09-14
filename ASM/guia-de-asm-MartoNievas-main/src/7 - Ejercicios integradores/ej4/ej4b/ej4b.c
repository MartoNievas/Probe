#include "ej4b.h"

#include <string.h>

// OPCIONAL: implementar en C
void invocar_habilidad(void *carta_generica, char *habilidad) {
  card_t *carta = (card_t *)carta_generica;
  if (carta == NULL) {
    return;
  }
  for (int i = 0; i < carta->__dir_entries; i++) {
    if (strcmp(carta->__dir[i]->ability_name, habilidad) == 0) {
      ((void (*)(void *))carta->__dir[i]->ability_ptr)(carta);
      return;
    }
  }
  if (carta->__archetype != NULL) {
    invocar_habilidad(carta->__archetype, habilidad);
    return;
  }
}
