#include <stdio.h>
int g = 10;
void functionA() {
  int a = 20;
  static int b = 30;
  printf("Dentro de functionA:\n");
  printf(" g = %d\n", g);
  printf(" a = %d\n", a);
  printf(" b = %d\n", b);
  // Modificaci´on de las variables
  g += 5;
  a += 10;
  b += 5;
}
void functionB() {
  int a = 40;
  static int c = 50;
  printf("\nDentro de functionB:\n");
  printf(" g = %d\n", g);
  printf(" a = %d\n", a);
  printf(" c = %d\n", c);
  // Modificaci´on de las variables
  g += 5;
  a += 10;
  c += 5;
}
int main() {
  printf("Dentro de main:\n");
  printf(" g = %d\n", g);
  functionA();
  functionB();
  functionA();
  functionB();
  printf("\nFinal en main:\n");
  printf(" g = %d\n", g);
  return 0;
}

/*

int b, c; variables globales
void f(void)
{
int b, d; locales
}
void g(int a)
{
int c; local
{
int a, d; locales
}
}


*/