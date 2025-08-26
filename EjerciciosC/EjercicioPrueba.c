#include <stdio.h>
#define FELIZ 0
#define TRISTE 1
// Esto parece un interfaz no?
// Como son todas variables locales y C por defecto siempre pasa por copia lo
// que impimir el programa es Estoy triste
// Pero si hacemos una modificacion para que las funciones reciban punteros y
// modificamos la posicion de memoria que se nos pasa como parametro podemos
// llegar a que el programa imprima Estoy feliz.
void ser_feliz(int estado);
void print_estado(int estado);
int main() {
  int estado = TRISTE; // automatic duration. Block scope
  ser_feliz(estado);
  print_estado(estado); // qu´e imprime?
}
void ser_feliz(int estado) { estado = FELIZ; }
void print_estado(int estado) {
  printf("Estoy %s\n", estado == FELIZ ? "feliz" : "triste");
}

// Ejemplo de como hacerlo con punteros
/*
void ser_feliz(int* estado);
void print_estado(int* estado);
int main() {
  int estado = TRISTE; // automatic duration. Block scope
  ser_feliz(&estado);
  print_estado(&estado); // qu´e imprime?
}
void ser_feliz(int* estado) { *estado = FELIZ; }
void print_estado(int* estado) {
  printf("Estoy %s\n", *estado == FELIZ ? "feliz" : "triste");
}
*/