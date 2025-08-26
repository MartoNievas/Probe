#include <stdio.h>
#include <stdlib.h>
// Incorrecta
void allocateArray(int *arr, int size, int value) {
  arr = (int *)malloc(size * sizeof(int));
  if (arr != NULL) {
    for (int i = 0; i < size; i++) {
      arr[i] = value;
    }
  }
}
// Funcion correcta
void allocateArray1(int **arr, int size, int value) {

  *arr = (int *)malloc(size * sizeof(int));
  if (arr != NULL) {
    for (int i = 0; i < size; i++) {
      (*arr)[i] = value;
    }
  }
}
// Uso
// Esto va a fallar debido a que arr tiene scope local y ademas arr es un
// puntero a un entero
int main() {
  int *vector = NULL;
  allocateArray1(&vector, 5, 45);
  for (int i = 0; i < 5; i++)
    printf("%d\n", vector[i]);
  free(vector);
}