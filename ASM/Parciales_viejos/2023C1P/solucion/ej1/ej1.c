#include "ej1.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

uint32_t cuantosTemplosClasicos_c(templo *temploArr, size_t temploArr_len) {
  if (temploArr == NULL) {
    return 0;
  }
  uint32_t res = 0;
  for (uint32_t i = 0; i < temploArr_len; i++) {
    if (temploArr[i].colum_corto * 2 + 1 == temploArr[i].colum_largo) {
      res++;
    }
  }
  return res;
}

templo *templosClasicos_c(templo *temploArr, size_t temploArr_len) {
  if (temploArr == NULL) {
    return NULL;
  }
  uint32_t cantidadDeTemplosClasicos =
      cuantosTemplosClasicos(temploArr, temploArr_len);
  templo *res = malloc(sizeof(templo) * cantidadDeTemplosClasicos);
  if (res == NULL) {
    return NULL;
  }
  uint32_t i1 = 0;
  for (uint32_t i = 0; i < temploArr_len; i++) {
    if (temploArr[i].colum_corto * 2 + 1 == temploArr[i].colum_largo) {
      res[i1] = temploArr[i];
      i1++;
    }
  }
  return res;
}
