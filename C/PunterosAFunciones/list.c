#include "headers/list.h"
#include "headers/type.h"
#include "stdio.h"
#include <stdlib.h>

funCopy_t getCopyFuntion(type_t t) {
  switch (t) {
  case TypeFAT32:
    return (funCopy_t)copy_fat32;
    break;
  case TypeEXT4:
    return (funCopy_t)copy_ext4;
    break;
  case TypeNTFS:
    return (funCopy_t)copy_ntfs;
    break;
  default:
    return NULL;
    break;
  }
}

funRm_t getRmFuntion(type_t t) {
  switch (t) {
  case TypeFAT32:
    return (funRm_t)rm_fat32;
    break;
  case TypeEXT4:
    return (funRm_t)rm_ext4;
    break;
  case TypeNTFS:
    return (funRm_t)rm_ntfs;
    break;
  default:
    return NULL;
    break;
  }
}

// Definicion de funciones
list_t *listNew(type_t t) {
  list_t *new_list = malloc(sizeof(list_t));

  if (new_list == NULL) {
    printf("ERROR: Al asignar memoria\n");
    return NULL;
  }
  // Asignamos atributos
  new_list->first = NULL;
  new_list->size = 0;
  new_list->type = t;
  printf("Lista creada con exito\n");
  return new_list;
}

// copia el dato
void listAddFirst(list_t *l, void *data) {
  if (l == NULL)
    return;
  node_t *n = malloc(sizeof(node_t));
  if (n == NULL) {
    printf("Error en listAddFirst\n");
    return;
  }
  n->data = getCopyFuntion(l->type)(data);
  n->next = l->first;
  l->first = n;
  l->size++;
  printf("Elemento agregado con exito\n");
}

// se asume: i < l->size
void *listGet(list_t *l, uint8_t i) {
  if (i >= l->size) {
    printf("ERROR: Indice fuera de rango\n");
    return NULL;
  }
  node_t *n = l->first;
  if (n == NULL) {
    printf("ERROR: la lista esta vacia");
    return NULL;
  }
  for (int k = 0; k < i; k++) {
    n = n->next;
  }
  return n->data;
}

// se asume: i < l->size
void *listRemove(list_t *l, uint8_t i) {
  if (i >= l->size) {
    printf("ERROR: Indice fuera de rango\n");
    return NULL;
  }
  if (l->size == 0) {
    printf("ERROR: la lista esta vacia\n");
    return NULL;
  }

  node_t *previous = NULL;
  node_t *current = l->first;
  for (int k = 0; k < i; k++) {
    previous = current;
    current = current->next;
  }
  // Ahora la logica de remover

  void *temp = current->data;

  // Caso especial: eliminar el primero
  if (previous == NULL) {
    l->first = current->next;
  } else {
    previous->next = current->next;
  }

  free(current);
  l->size--;

  return temp;
}

void listDelete(list_t *l) {
  if (l == NULL) {
    printf("ERROR: puntero nulo");
    return;
  }
  node_t *temp = NULL;
  node_t *n = l->first;
  if (n == NULL) {
    printf("ERROR: Null Pointer");
    return;
  }

  while (n != NULL) {

    temp = n;
    n = n->next;
    getRmFuntion(l->type)(temp->data);
    free(temp);
  }
  free(l);
}

void listSwap(list_t *l, uint8_t i, uint8_t j);

void print(list_t *l) {
  if (l == NULL) {
    printf("ERROR: Null Pointer");
    return;
  }
  node_t *n = l->first;
  if (n == NULL) {
    printf("ERROR: la lista esta vacia");
    return;
  }
  for (int i = 0; i < l->size; i++) {
    printf("Elemento %d = %p\n", i, n->data);
    n = n->next;
  }
}