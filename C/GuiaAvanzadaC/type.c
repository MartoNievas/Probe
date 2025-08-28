#include "type.h"
#include "stdio.h"
#include <stdlib.h>

fat32_t *new_fat32() {
  fat32_t *file = (fat32_t *)malloc(sizeof(fat32_t));
  if (file == NULL) {
    printf("ERROR: no se asgino correctamente la memoria\n");
    return NULL;
  }
  *file = 0; // Asignamos 0 espacio
  printf("Se creo correctamente el archivo\n");
  return file;
}

ext4_t *new_ext4() {
  fat32_t *file = (ext4_t *)malloc(sizeof(ext4_t));
  if (file == NULL) {
    printf("ERROR: no se asgino correctamente la memoria\n");
    return NULL;
  }
  *file = 0; // Asignamos 0 espacio
  printf("Se creo correctamente el archivo\n");
  return file;
}
ntfs_t *new_ntfs() {
  fat32_t *file = (ntfs_t *)malloc(sizeof(ntfs_t));
  if (file == NULL) {
    printf("ERROR: no se asgino correctamente la memoria\n");
    return NULL;
  }
  *file = 0; // Asignamos 0 espacio
  printf("Se creo correctamente el archivo\n");
  return file;
}

fat32_t *copy_fat32(fat32_t *file) {
  if (file == NULL) {
    printf("El archivo que se paso no es valido");
    return NULL;
  }

  fat32_t *newFile = (fat32_t *)malloc(sizeof(file));
  if (newFile == NULL) {
    printf("ERROR: Al asignar memoria\n");
    return NULL;
  }
  return newFile;
}

ext4_t *copy_ext4(ext4_t *file) {
  if (file == NULL) {
    printf("El archivo que se paso no es valido");
    return NULL;
  }

  ext4_t *newFile = (ext4_t *)malloc(sizeof(file));
  if (newFile == NULL) {
    printf("ERROR: Al asignar memoria\n");
    return NULL;
  }
  return newFile;
}

ntfs_t *copy_ntfs(ntfs_t *file) {
  if (file == NULL) {
    printf("El archivo que se paso no es valido");
    return NULL;
  }

  ntfs_t *newFile = (ntfs_t *)malloc(sizeof(file));
  if (newFile == NULL) {
    printf("ERROR: Al asignar memoria\n");
    return NULL;
  }
  return newFile;
}

void rm_fat32(fat32_t *file) {
  if (file == NULL) {
    printf("El archivo no existe");
    return;
  }
  free(file);
  printf("Archivo eliminado con exito");
}

void rm_ext4(ext4_t *file) {
  if (file == NULL) {
    printf("El archivo no existe");
    return;
  }
  free(file);
  printf("Archivo eliminado con exito");
}

void rm_ntfs(ntfs_t *file) {
  if (file == NULL) {
    printf("El archivo no existe");
    return;
  }
  free(file);
  printf("Archivo eliminado con exito");
}