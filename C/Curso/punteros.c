#include "stdio.h"

void jugar(int n) {
  n = (n + 4) * 8 - 1;
  printf("%d\n", n);
}

void hacer_algo(int *dirr) {
  int y = *dirr; // con esto recupero el valor que hay en el puntero
  y = (y + 2) * 4;
  *dirr = y; // Aqui lo que estoy diciendo es que en la direccion n modifico por
             // el valor y
  printf("El valor dse x ahora es: %d\n", y);
}

int main() {
  int x = 20;
  // Con &nombre_variable devolvemos la posicion de memoria donde se encuentra

  int *dirX = &x; // Con esto guardamos en un puntero la direccion de memoria de
                  // x, denotamos un puntero como tipo_dato*
  // en C cuando le pasamos un parametro a la funcion siempre se pasa por copia,
  // no por referencia
  // Aqui un ejemplo
  jugar(x);
  printf("x = %d\n", x);

  // Cuando trabajamos con puntero, estamos trabajando con direcciones de
  // memoria entonces los parametros se pasan por referencia por ejemplo

  // si hago una funcion que tome un puntero, vamos a ver como se modifica el
  // valor de x despues de la ejecucion de la funcion
  // ya que modifique la direccion de memoria donde se enecuentra x.
  printf("Valor modificado de x\n");
  hacer_algo(dirX);
  printf("x = %d\n", x);
}