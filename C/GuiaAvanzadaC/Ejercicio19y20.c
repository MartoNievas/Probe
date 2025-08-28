#include <stdint.h>
#include <stdio.h>
#define VIDEO_COLS 80
#define VIDEO_FILS 50
int main() {
  int matrix[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
  // p apunta al int en la fila 0, columna 0 no. Apunta a la direccion de
  // memoria en la fila 0 columna 0
  int *p = &matrix[0][0];
  // ¿qu´e es reshape? reinterpreta la memoria de una matriz
  int (*reshape)[2] = (int (*)[2])p;
  printf("%d\n", p[3]);          // Qu´e imprime esta l´ınea? imprime 4
  printf("%d\n", reshape[1][1]); // Qu´e imprime esta l´ınea? imprime 4
  return 0;
}

// Cada posicion de memoria tiene 2 bytes
typedef struct ca_s {
  uint8_t c; // caracter
  uint8_t a; // atributos
} ca;

volatile uint16_t *const VIDEO = (volatile uint16_t *)0xB8000;

void screen_draw_layout(void) {
  // VIDEO es un puntero a la direcci´on de memoria del buffer de video
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  uint32_t f, c;
  for (f = 0; f < VIDEO_FILS; f++) {
    for (c = 0; c < VIDEO_COLS; c++) {
      p[f][c].c = ' ';
      p[f][c].a = 0x10;
    }
  }
}