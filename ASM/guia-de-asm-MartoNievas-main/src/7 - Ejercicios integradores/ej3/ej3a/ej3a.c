#include "../ejs.h"
#include <stdlib.h>
#include <unistd.h>

// Función auxiliar para contar casos por nivel
int contar_casos_por_nivel(caso_t *arreglo_casos, int largo, int nivel) {
  if (arreglo_casos == NULL) {
    return -1;
  }
  int res = 0;
  for (int i = 0; i < largo; i++) {
    if (arreglo_casos[i].usuario->nivel == nivel) {
      res += 1;
    }
  }
  return res;
}

segmentacion_t *segmentar_casos(caso_t *arreglo_casos, int largo) {
  if (arreglo_casos == NULL) {
    return NULL;
  }
  int nivel0 = contar_casos_por_nivel(arreglo_casos, largo, 0);
  int nivel1 = contar_casos_por_nivel(arreglo_casos, largo, 1);
  int nivel2 = contar_casos_por_nivel(arreglo_casos, largo, 2);

  segmentacion_t *res = malloc(sizeof(segmentacion_t));

  if (res == NULL) {
    return NULL;
  }

  // No me importa si alguno es null
  caso_t *casos_nivel0 = nivel0 > 0 ? malloc(sizeof(caso_t) * (nivel0)) : NULL;
  caso_t *casos_nivel1 = nivel1 > 0 ? malloc(sizeof(caso_t) * (nivel1)) : NULL;
  caso_t *casos_nivel2 = nivel2 > 0 ? malloc(sizeof(caso_t) * (nivel2)) : NULL;

  res->casos_nivel_0 = casos_nivel0;
  res->casos_nivel_1 = casos_nivel1;
  res->casos_nivel_2 = casos_nivel2;

  int i0 = 0, i1 = 0, i2 = 0;
  for (int i = 0; i < largo; i++) {
    if (arreglo_casos[i].usuario->nivel == 0) {
      res->casos_nivel_0[i0++] = arreglo_casos[i];
    } else if (arreglo_casos[i].usuario->nivel == 1) {
      res->casos_nivel_1[i1++] = arreglo_casos[i];
    } else if (arreglo_casos[i].usuario->nivel == 2) {
      res->casos_nivel_2[i2++] = arreglo_casos[i];
    }
  }
  return res;
}
