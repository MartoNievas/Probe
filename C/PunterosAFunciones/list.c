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
    perror("Error en listAddFirst");
    return;
    n->data = getCopyFuntion(l->type)(data);
    n->next = l->first;
    l->first = n;
    l->size++;
    printf("Elemento agregado con exito\n");
  }
}

// se asume: i < l->size
void *listGet(list_t *l, uint8_t i) { return NULL; }

// se asume: i < l->size
void *listRemove(list_t *l, uint8_t i) { return NULL; }

void listDelete(list_t *l);