#include "stdio.h"
#include "string.h"

// Vamos con los unions
union mi_union_t {
  // Codigo
  // Campos
  // La diferecencia con los strcuts es que todos los campos variables apuntan
  // al mismo espacio de memoria es decir que si asigno entero y luego flotante
  // se sobreescribe entero
  int entero;
  float flotante;
};

// vamos con enum, sirve para construir un conjunto de identificadores similar a
// java pero con muchas funcionalidades menos

enum mi_enum_t { ROJO, VERDE, AZUL };

// Con typedef podemos cambiar la forma en las que se declaran unions, enums,
// structs, ect.
typedef union mi_union_t Union;

// y asi donde hagomos referencia al tipo mi_union_t podemos llamarlos Union

void imprimir_numero(Union u) { printf("El numero es: %d\n", u.entero); }

int main() {
  // Union ejemplo
  Union u;
  u.entero = 123252623;

  printf("Entero es %d\n", u.entero);

  // Ahora vamos con enums

  enum mi_enum_t e;
  e = ROJO; // le asigno una constante declarada anteriormente

  // typedef
  imprimir_numero(u);

  return 0;
}