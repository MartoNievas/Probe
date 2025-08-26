#include <stdio.h>
#define FELIZ 0
#define TRISTE 1
int estado = TRISTE; // static duration. File scope
void alcoholizar();
void print_estado();
int main() {
  print_estado();
  alcoholizar();
  print_estado();
  alcoholizar();
  alcoholizar();
  alcoholizar();
  print_estado(); // que imprime?
  // Por lo tanto va a imprimir Estoy triste
}
void alcoholizar() {
  static int cantidad =
      0; // static duration. block scope //Va a conservar el valor
  cantidad++;
  if (cantidad < 3) {
    estado = FELIZ;
  } else {
    estado = TRISTE;
  }
}
void print_estado() {
  printf("Estoy %s\n", estado == FELIZ ? "feliz" : "triste");
}
// Con la keyword static al estar en una variable local de la declaracion de una
// funcion lo que va a pasar es que el valor se inicializa en el primer llamado
// y queda guardado para llamados posteriores

// Si la keyword static se hace en una variable global lo que pasa es que queda
// accesible solo para el archivo donde fue declarada

// Si la keyword static se le pone a un funcion, esa funcion solo puede ser
// usada dentro del archivo.c