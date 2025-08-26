#include <stdio.h>
#include <stdlib.h> //Con esta libreria podemos trabajar con memoria dinamica
int main() {

  // Por ejemplo un arreglo de 4 mil flotantes hacemos la cantidad de elemtnos
  // por su tamano con sizeof Por ultimo lo guardamos en un puntero y lo
  // casteamos usando (float*)
  float *valores = (float *)malloc(
      4000 * sizeof(float)); // Con este podemos reservar memoria

  // En versiones viejas de C no tenes VLAS entonces debemos reservar memoria de
  // manera dinamica para imitar el comportamiento de los VLAS

  // Ejemplo de VLA
  printf("Cuantos elemtos va a tener el arreglo\n");

  int tam;
  scanf("%d",
        &tam); // indicamos simepre la posicion de memoria en tam o longitud

  float *arr = (float *)malloc(tam * sizeof(float));
  if (arr == NULL) {
    printf("No se asigno corretamente la memoria");
    return 0;
  }
  // Despues de crearlo podemos reccorrelos usando aritmetica de punteros
  for (int i = 0; i < tam; i++) {
    printf("Ingrese el numero %d: ", i + 1);
    scanf("%f", (arr + i));
  }

  for (int i = 0; i < tam; i++) {
    printf("%.2f\n", arr[i]);
  }

  free(arr); // Asi liberamos memoria
  return 0;
}