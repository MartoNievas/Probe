#pragma once

#define EJ2
#include "../ej4a/ej4a.h"

typedef struct {
  directory_t __dir;      // 0
  uint16_t __dir_entries; // 8
  void *__archetype;      // 16
} card_t;                 // 24

typedef struct {
  directory_t __dir;
  uint16_t __dir_entries;
  fantastruco_t *__archetype;
  uint32_t materials;
} alucard_t;

typedef void ability_function_t(void *card);

void invocar_habilidad(void *carta, char *habilidad);
