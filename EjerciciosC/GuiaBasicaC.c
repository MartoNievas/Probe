#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include <stdint.h>
void ejercicio1() {
  printf("Enteros tamano: \n");
  printf("char tamono: %lu bytes\n", sizeof(char));
  printf("short tamono: %lu bytes\n", sizeof(short));
  printf("int tamono: %lu bytes\n", sizeof(int));
  printf("long tamono: %lu bytes\n", sizeof(long));
  printf("long tamono: %lu bytes\n", sizeof(long));
  printf("long long tamono: %lu bytes\n", sizeof(long long));

  printf("Reales tamano: \n");
  printf("float tamano: %lu bytes\n", sizeof(float));
  printf("double tamano: %lu bytes\n", sizeof(double));
  printf("long double tamano: %lu bytes\n", sizeof(long double));

  printf("Char tamano: \n");
  printf("chat tamano: %lu bytes\n", sizeof(char));

  printf("Punteros tamano: \n");
  printf("int* tamano: %lu bytes \n", sizeof(int *));
  printf("char* tamano: %lu bytes \n", sizeof(char *));
  printf("void* tamano: %lu bytes \n", sizeof(void *));
  // Varios mas

  printf("Booleano tamano: \n");
  printf("bool tamano: %lu bytes \n", sizeof(bool));
}

void ejercicio2() {
  printf("Signed size: \n");
  printf("int8_t tamamo: %lu bytes \n", sizeof(int8_t));
  printf("int16_t tamamo: %lu bytes \n", sizeof(int16_t));
  printf("int32_t tamamo: %lu bytes \n", sizeof(int32_t));
  printf("int64_t tamamo: %lu bytes \n", sizeof(int64_t));

  printf("Unsigned size: \n");
  printf("uint8_t tamamo: %lu bytes \n", sizeof(uint8_t));
  printf("uint16_t tamamo: %lu bytes \n", sizeof(uint16_t));
  printf("uint32_t tamamo: %lu bytes \n", sizeof(uint32_t));
  printf("uint64_t tamamo: %lu bytes \n", sizeof(uint64_t));
}

void ejercicio3() {
  float num1 = 0.1;
  double num2 = 0.1;

  printf("Casteo de flotante a entero: %d\n", (int)num1);
  printf("Casteo de double a entero: %d\n", (int)num2);
}

void mensaje_secreto() {
  int mensaje_secreto[] = {
      116, 104, 101, 32,  103, 105, 102, 116, 32,  111, 102, 32,  119, 111,
      114, 100, 115, 32,  105, 115, 32,  116, 104, 101, 32,  103, 105, 102,
      116, 32,  111, 102, 32,  100, 101, 99,  101, 112, 116, 105, 111, 110,
      32,  97,  110, 100, 32,  105, 108, 108, 117, 115, 105, 111, 110};
  size_t length = sizeof(mensaje_secreto) / sizeof(int);
  char decoded[length];
  for (int i = 0; i < length; i++) {
    decoded[i] = (char)(mensaje_secreto[i]); // casting de int a char
  }
  for (int i = 0; i < length; i++) {
    printf("%c", decoded[i]);
  }
}

void comprar_regiones_de_palabras() {

  uint32_t w1, w2;

  printf("Ingrese el primer dato: \n");
  scanf("%X", &w1);

  printf("Ingrese el segundo dato: \n");
  scanf("%X", &w2);

  // Siftheamos a la derecha y aplicamos la mascara 000000...00111 en binario o
  // en hexa
  uint32_t high3 = (w1 >> 29) & 0b111;

  uint32_t low3 = w2 & 0b111;

  if (high3 == low3) {
    printf("Ambas regiones de las palabras coinciden.\n");
  } else {
    printf("No coinciden las regiones de ambas palabras.\n");
  }
}

void rotacion_izq(int arr[], int tam) {

  if (tam == 0) {
    printf("El array esta vacio");
  }
  printf("Orginal: \n");
  for (int i = 0; i < tam; i++) {
    printf("Elemento %d: %d\n", i, arr[i]);
  }
  int i;
  int first = arr[0];
  for (i = 0; i < tam - 1; i++) {
    arr[i] = arr[i + 1];
  }
  arr[tam - 1] = first;
  printf("Rotado:\n");
  for (int i = 0; i < tam; i++) {
    printf("Elemento %d: %d \n", i, arr[i]);
  }
}
void k_rotaciones_izq(int arr[], int k, int tam) {

  printf("Orginal:\n");
  for (int i = 0; i < tam; i++) {
    printf("Elemento %d: %d\n", i, arr[i]);
  }

  // Ajustamos el k ya que rotar n veces es como no rotar
  k = k % tam;

  int temp[k];
  for (int i = 0; i < k; i++) {
    temp[i] = arr[i];
  }

  for (int i = 0; i < tam - k; i++) {
    arr[i] = arr[i + k];
  }

  for (int i = 0; i < k; i++) {
    arr[tam - k - i] = temp[i];
  }

  printf("Rotado %d veces:\n", k);
  for (int i = 0; i < tam; i++) {
    printf("Elemento %d: %d\n", i, arr[i]);
  }
}
void contar_caras() {
  int caras[6] = {0, 0, 0, 0, 0, 0};
  int random;
  for (int i = 0; i < 60000001; i++) {
    random = rand() % 7;
    caras[random - 1] += 1;
  }
  for (int i = 0; i < 6; i++) {
    printf("la cara %d salio %d\n", i + 1, caras[i]);
  }
}

int factorial_interativo(int n) {
  int res = 1;
  for (int i = 1; i <= n; i++) {
    res = res * i;
  }
  printf("%d\n", res);
  return res;
}

int factorial_recursivo(int n) {

  if (n <= 1) {
    return 1;
  }
  return n * factorial_recursivo(n - 1);
}

int main() { printf("%d\n", factorial_recursivo(5)); }