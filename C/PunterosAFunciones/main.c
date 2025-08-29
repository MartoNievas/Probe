#include "headers/list.h"
#include "stdio.h"

int main() {
  int x = 10;
  int *p = &x;
  printf("Hola Mundo\n");
  list_t *l = listNew(TypeFAT32);
  listAddFirst(l, p);
  print(l);
  int *elem = listGet(l, 0);
  printf("%d\n", *elem);
  int *elem1 = listRemove(l, 0);
  printf("%d\n", *elem1);
  return 0;
}