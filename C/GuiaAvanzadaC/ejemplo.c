#include "ejemplo.h"
#include "stdio.h"
#include <stdint.h>
#include <stdlib.h>

uint16_t *secuencia(uint16_t n) {
  uint16_t *arr = (uint16_t *)malloc(
      sizeof(uint16_t) * n); // con malloc asigno memoria para un arreglo
  if (arr == NULL) {
    printf("Error: no se pudo asignar memoria");
    return NULL;
  }
  for (int i = 0; i < n; i++) {
    arr[i] = i;
  }

  return arr;
}
