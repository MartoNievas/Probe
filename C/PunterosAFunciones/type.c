#include "headers/type.h"
#include <stdio.h>
#include <stdlib.h>

// ====== Constructores ======

fat32_t *new_fat32() {
  fat32_t *file = (fat32_t *)malloc(sizeof(fat32_t));
  if (file == NULL) {
    printf("ERROR: no se asignó correctamente la memoria\n");
    return NULL;
  }
  *file = 0; // Inicializamos
  printf("Se creó correctamente el archivo FAT32\n");
  return file;
}

ext4_t *new_ext4() {
  ext4_t *file = (ext4_t *)malloc(sizeof(ext4_t));
  if (file == NULL) {
    printf("ERROR: no se asignó correctamente la memoria\n");
    return NULL;
  }
  *file = 0;
  printf("Se creó correctamente el archivo EXT4\n");
  return file;
}

ntfs_t *new_ntfs() {
  ntfs_t *file = (ntfs_t *)malloc(sizeof(ntfs_t));
  if (file == NULL) {
    printf("ERROR: no se asignó correctamente la memoria\n");
    return NULL;
  }
  *file = 0;
  printf("Se creó correctamente el archivo NTFS\n");
  return file;
}

// ====== Copias ======

fat32_t *copy_fat32(fat32_t *file) {
  if (file == NULL) {
    printf("El archivo que se pasó no es válido\n");
    return NULL;
  }

  fat32_t *newFile = (fat32_t *)malloc(sizeof(fat32_t));
  if (newFile == NULL) {
    printf("ERROR: Al asignar memoria\n");
    return NULL;
  }

  *newFile = *file; // Copiamos el valor
  return newFile;
}

ext4_t *copy_ext4(ext4_t *file) {
  if (file == NULL) {
    printf("El archivo que se pasó no es válido\n");
    return NULL;
  }

  ext4_t *newFile = (ext4_t *)malloc(sizeof(ext4_t));
  if (newFile == NULL) {
    printf("ERROR: Al asignar memoria\n");
    return NULL;
  }

  *newFile = *file;
  return newFile;
}

ntfs_t *copy_ntfs(ntfs_t *file) {
  if (file == NULL) {
    printf("El archivo que se pasó no es válido\n");
    return NULL;
  }

  ntfs_t *newFile = (ntfs_t *)malloc(sizeof(ntfs_t));
  if (newFile == NULL) {
    printf("ERROR: Al asignar memoria\n");
    return NULL;
  }

  *newFile = *file;
  return newFile;
}

// ====== Eliminación ======

void rm_fat32(fat32_t *file) {
  if (file == NULL) {
    printf("El archivo no existe\n");
    return;
  }
  free(file);
  printf("Archivo FAT32 eliminado con éxito\n");
}

void rm_ext4(ext4_t *file) {
  if (file == NULL) {
    printf("El archivo no existe\n");
    return;
  }
  free(file);
  printf("Archivo EXT4 eliminado con éxito\n");
}

void rm_ntfs(ntfs_t *file) {
  if (file == NULL) {
    printf("El archivo no existe\n");
    return;
  }
  free(file);
  printf("Archivo NTFS eliminado con éxito\n");
}
