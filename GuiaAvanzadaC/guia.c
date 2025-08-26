#include "stdio.h"
#include <stdint.h>
#include <stdlib.h>

struct monstruo_t {
  char *nombre;
  int vida;
  double ataque;
  double defensa;
};

typedef struct monstruo_t monstruo;

// Definimos los monstruos

void imprimir_estadisticas(monstruo m[], int tam) {
  int i;
  for (i = 0; i < tam; i++) {
    printf("El nombre del monstruo es %s y tiene %d puntos de vida\n",
           m[i].nombre, m[i].vida);
  }
}

monstruo evolucionar(monstruo *m) {
  monstruo nuevo_monstruo;
  nuevo_monstruo.nombre = m->nombre;
  nuevo_monstruo.vida = m->vida;
  nuevo_monstruo.ataque = (m->ataque) + 10;
  nuevo_monstruo.defensa = (m->defensa) + 10;
  return nuevo_monstruo;
}

void imprimir_stats(monstruo *m) {
  printf("Nombre: %s\nVida: %d \nAtaque: %f \nDefensa: %f \n", m->nombre,
         m->vida, m->ataque, m->defensa);
}

void swap(int *x, int *y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}

void pasar_a_mayusculas(char *str) {
  int i = 0;
  while (str[i] != '\0') {
    if (str[i] >= 'a' && str[i] <= 'z') {
      str[i] = (('A' - 'a') + str[i]);
    }
    i++;
  }
}

struct persona_t {
  char *nombre;
  uint16_t edad;
};
typedef struct persona_t persona_t;

// definimos la funcion para asignar un nombre

persona_t *crear_persona(char *nombre, uint16_t edad) {
  // reservamos memoria dinamica
  persona_t *p = malloc(sizeof(persona_t));
  p->nombre = nombre;
  p->edad = edad;

  return p;
}

void eliminar_persona(persona_t *p) {
  char *nombre = p->nombre;
  free(p);
  printf("La persona %s fue eliminada del sistema\n", nombre);
}

int main() {
  monstruo monstruos[] = {
      {"Dragón Rojo", 300, 75.5, 60.0}, {"Orco", 120, 40.0, 25.0},
      {"Goblin", 60, 15.0, 10.0},       {"Troll", 200, 50.0, 35.0},
      {"Esqueleto", 80, 20.0, 15.0},    {"Vampiro", 150, 45.0, 30.0},
      {"Hidra", 500, 90.0, 70.0}};
  int tam = 7;
  imprimir_estadisticas(monstruos, tam);

  // definimos un monstruo
  printf("Sin evolucionar:\n");
  monstruo m = {"Zombie", 200, 300, 400};
  imprimir_stats(&m);
  monstruo m_evo = evolucionar(&m);
  printf("Evolucionado: \n");
  imprimir_stats(&m_evo);

  int x = 10, y = 20;
  printf("Antes del sweap: x = %d, y = %d \n", x, y);
  swap(&x, &y);
  printf("Despues del sweap: x = %d, y = %d \n", x, y);

  char str[4] = "hola";

  printf("Antes: %s\n", str);
  pasar_a_mayusculas(str);
  printf("Despues: %s\n", str);

  // Ejercicio persona_t
  persona_t *p = crear_persona(
      "Martin",
      20); // creamos un puntero a la memoria donde se almacena persona_t
  printf("el nombre es %s y la edad %d\n", p->nombre, p->edad);
  free(p); // liberamos la memoria

  /*
    printf(("el nombre es %s y la edad %d\n"), p->nombre,
           p->edad);  si lo intento imprimir ya no esta mas en memoria una vez
    libero
  */

  persona_t *p1 = crear_persona("Martin", 21);
  printf("el nombre es %s y la edad %d\n", p1->nombre, p1->edad);
  eliminar_persona(p1);
  return 0;
}
