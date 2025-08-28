#include "list.h"
#include "type.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

list_t *listNew(type_t t) {
  list_t *l = malloc(sizeof(list_t));
  if (l == NULL) {
    perror("Error en listNew");
    return NULL;
  }
  l->type = t;
  l->size = 0;
  l->first = NULL;
  return l;
}

void listAddFirst(list_t *l, void *data) {
  if (l == NULL)
    return;
  node_t *n = malloc(sizeof(node_t));
  if (n == NULL) {
    perror("Error en listAddFirst");
    return;
  }
  switch (l->type) {
  case TypeFAT32:
    n->data = (void *)copy_fat32((fat32_t *)data);
    break;
  case TypeEXT4:
    n->data = (void *)copy_ext4((ext4_t *)data);
    break;
  case TypeNTFS:
    n->data = (void *)copy_ntfs((ntfs_t *)data);
    break;
  }
  n->next = l->first;
  l->first = n;
  l->size++;
}

void *listGet(list_t *l, uint8_t i) {
  if (l == NULL || i >= l->size)
    return NULL;
  node_t *n = l->first;
  for (uint8_t j = 0; j < i; j++)
    n = n->next;
  return n->data;
}

void *listRemove(list_t *l, uint8_t i) {
  if (l == NULL || i >= l->size)
    return NULL;
  node_t *tmp = NULL;
  void *data = NULL;
  if (i == 0) {
    data = l->first->data;
    tmp = l->first;
    l->first = l->first->next;
  } else {
    node_t *n = l->first;
    for (uint8_t j = 0; j < i - 1; j++)
      n = n->next;
    data = n->next->data;
    tmp = n->next;
    n->next = n->next->next;
  }
  free(tmp);
  l->size--;
  return data;
}

void listDelete(list_t *l) {
  if (l == NULL)
    return;
  node_t *n = l->first;
  while (n) {
    node_t *tmp = n;
    n = n->next;
    switch (l->type) {
    case TypeFAT32:
      rm_fat32((fat32_t *)tmp->data);
      break;
    case TypeEXT4:
      rm_ext4((ext4_t *)tmp->data);
      break;
    case TypeNTFS:
      rm_ntfs((ntfs_t *)tmp->data);
      break;
    }
    free(tmp);
  }
  free(l);
}

static void prevycurr(node_t **prev, node_t **curr, uint8_t i, list_t *l) {
  *prev = NULL;
  *curr = l->first;
  for (uint8_t k = 0; k < i; k++) {
    *prev = *curr;
    *curr = (*curr)->next;
  }
}

void swap(uint8_t i, uint8_t j, list_t *l) {
  if (l == NULL || l->size <= 1 || i == j) {
    return;
  }
  if (i >= l->size || j >= l->size) {
    printf("Indices invalidos\n");
    return;
  }
  if (i > j) {
    uint8_t temp = i;
    i = j;
    j = temp;
  }

  node_t *niprev, *nicurr, *njprev, *njcurr;
  prevycurr(&niprev, &nicurr, i, l);
  prevycurr(&njprev, &njcurr, j, l);

  // Caso: Nodos adyacentes
  if (nicurr->next == njcurr) {
    if (niprev != NULL) {
      niprev->next = njcurr;
    } else {
      l->first = njcurr;
    }
    nicurr->next = njcurr->next;
    njcurr->next = nicurr;
    return;
  }

  // Caso: Nodos no adyacentes
  if (niprev != NULL) {
    niprev->next = njcurr;
  } else {
    l->first = njcurr;
  }
  if (njprev != NULL) {
    njprev->next = nicurr;
  }

  node_t *temp = nicurr->next;
  nicurr->next = njcurr->next;
  njcurr->next = temp;
}