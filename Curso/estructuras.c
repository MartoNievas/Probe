#include "stdio.h"
#include "string.h" //sirve para trabajar con cadenas de caracteres
// Vamos a ver para que sirven las estructuras
// Sirven para agrupar variables

struct cuenta {
  // Aqui escribimos el codigo de nuestra estructura
  char nombre[30];
  char apellido[40];
  int id;
  int saldo;
};

// Lo normal es pasar las struct como punteros porque es mas eficiente

// Hay dos formas de acceder a las variables de un struct cuando se pasa como
// puntero
// 1. desreferenciando el puntero haciendo (*c.nombre)
// 2. cambiando el c.saldo por c->saldo
void datos(struct cuenta *c) {
  printf("Los datos de la cuenta son: \n");
  printf("{Nombre: %s, Apellido: %s, Identidicador: %d, Saldo: %d}\n",
         c->nombre, c->apellido, c->id, c->saldo);
}

void comprar(int precio, struct cuenta *c) {
  if (c->saldo >= precio) {
    printf("El articulo fue comprado con exito.\n");
    c->saldo -= precio;
  }
  printf("No tienes dinero suficiente en cuenta\n");
}

void ingresar_dinero(struct cuenta *c, int monto) {
  printf("el monto de %d fue ingresado a la cuenta de %s %s", monto, c->nombre,
         c->apellido);
  c->saldo += monto;
}

int main() {
  struct cuenta c1; // asi declaramos un varible de tipo cuenta

  c1.saldo = 400;
  c1.id = 214153;
  strcpy(c1.nombre, "Martin");
  strcpy(c1.apellido, "Nievas Wilberger");

  // Le paso como parametro el puntero
  datos(&c1);
  // Eso si hay que tener cuaidado porque si modifico campos del strcut dentro
  // de la funcion al ser pasado por referencia me lo modifica
  comprar(2000, &c1);
  printf("Datos luego de la compra: \n");
  datos(&c1);

  printf("Cuenta luego de ingresar un monto: \n");
  ingresar_dinero(&c1, 10000);
  datos(&c1);
  return 0;
}