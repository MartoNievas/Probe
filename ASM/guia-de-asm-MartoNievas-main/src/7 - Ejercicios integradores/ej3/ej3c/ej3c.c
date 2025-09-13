#include "../ejs.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void actualizar_estadistica(caso_t *caso, estadisticas_t *res) {
  if (caso == NULL || res == NULL) {
    return;
  }
  if (strncmp(caso->categoria, "RBO", 4) == 0) {
    res->cantidad_RBO++;
  }
  if (strncmp(caso->categoria, "CLT", 4) == 0) {
    res->cantidad_CLT++;
  }
  if (strncmp(caso->categoria, "KSC", 4) == 0) {
    res->cantidad_KSC++;
  }
  if (strncmp(caso->categoria, "KDT", 4) == 0) {
    res->cantidad_KDT++;
  }
  if (caso->estado == 0) {
    res->cantidad_estado_0++;
  }
  if (caso->estado == 1) {
    res->cantidad_estado_1++;
  }
  if (caso->estado == 2) {
    res->cantidad_estado_2++;
  }
}

estadisticas_t *calcular_estadisticas(caso_t *arreglo_casos, int largo,
                                      uint32_t usuario_id) {
  if (arreglo_casos == NULL) {
    return NULL;
  }
  estadisticas_t *res = malloc(sizeof(estadisticas_t));
  if (res == NULL) {
    return NULL;
  }
  res->cantidad_CLT = 0;
  res->cantidad_RBO = 0;
  res->cantidad_KDT = 0;
  res->cantidad_KSC = 0;
  res->cantidad_estado_0 = 0;
  res->cantidad_estado_1 = 0;
  res->cantidad_estado_2 = 0;
  if (usuario_id != 0) {
    for (int i = 0; i < largo; i++) {
      caso_t curr = arreglo_casos[i];
      if (curr.usuario->id == usuario_id) {
        actualizar_estadistica(&curr, res);
      }
    }
  } else if (usuario_id == 0) {
    for (int i = 0; i < largo; i++) {
      caso_t curr = arreglo_casos[i];
      actualizar_estadistica(&curr, res);
    }
  }
  return res;
}
