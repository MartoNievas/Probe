#include <stdio.h>

void imprimir_datos(int edad, char *nombre) {
  printf("nombre: %s, edad: %d", nombre, edad);
}
int main() {
  for (int i = 0; i < 10; i++) {
    printf("%d\n", i);
  }

  unsigned int numero = 1;
  int pares[5] = {2, 4, 6, 8, 10};
  // Para obtener la cantidad de elementos de un arreglo
  // se debe dvividir el tamano del bytes del arreglo con el
  // tamano de bytes de algun elementos.
  int n = sizeof(pares) / sizeof(pares[0]);
  printf("Aqui comienzan los pares:\n");
  for (int i = 0; i < n; i++) {
    printf("%d\n", pares[i]); // Para ver el tado \n
  }

  printf("\n");

  imprimir_datos(20, "Martin");
  return 0;
}