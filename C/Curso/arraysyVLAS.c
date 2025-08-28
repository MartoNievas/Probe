#include "stdio.h"

// C no es capaz de saber la cantidad de elementos de un array sin que se lo
// especifiques
float promedio(int *arr, int tam) {
  int i;
  float suma;
  for (i = 0; i < tam; i++) {
    suma += arr[i];
  }
  if (i != 0) {
    return suma / tam;
  }
  return 0.0;
}

void arrays() {

  int edades[8];
  for (int i = 0; i < 8; i++) {
    printf("Introduce en edades  %d:  ", i + 1);
    scanf("%d", &edades[i]); // Aqui dijo que quiero escribir en la posicion del
                             // i-esimo elementos de edades
  }

  for (int i = 0; i < 8; i++) {
    printf("Las edades son: %d\n", edades[i]);
  }
}

// A partir de una variable que se le pasa como parametro puedo decidir cuantos
// elementos voy a tener en el arreglo

void vLas() {

  printf("Cuantos elemetos vas a querer almacenar: \n");
  int tam;
  scanf("%d", &tam); // siempre a scanf debo pasarle la direccion de memoria de
                     // una variable
  int edades[tam];
  for (int i = 0; i < tam; i++) {
    printf("Introduce en edades  %d:  ", i + 1);
    scanf("%d", &edades[i]); // Aqui dijo que quiero escribir en la posicion del
                             // i-esimo elementos de edades
  }

  for (int i = 0; i < tam; i++) {
    printf("Las edades son: %d\n", edades[i]);
  }
  printf("La media de edad es: %f: ", promedio(edades, tam));
}

// Dato de color C interpreta los arrays como punteros entonces cuando nosotros
// le pasamos un array como parametro a una funcion realemte le estamos pasando
// la direccion de memoria del array
int main() {

  vLas();
  return 0;
}