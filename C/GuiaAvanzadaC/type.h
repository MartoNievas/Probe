#ifndef EJEMPLO_H
#define EJEMPLO_H

#include <stdint.h>
#include <stdlib.h>
typedef enum e_type { TypeFAT32 = 0, TypeEXT4 = 1, TypeNTFS = 2 } type_t;

typedef uint32_t fat32_t;
typedef uint32_t ext4_t;
typedef uint32_t ntfs_t;

fat32_t *new_fat32();
ext4_t *new_ext4();
ntfs_t *new_ntfs();
fat32_t *copy_fat32(fat32_t *file);
ext4_t *copy_ext4(ext4_t *file);
ntfs_t *copy_ntfs(ntfs_t *file);
void rm_fat32(fat32_t *file);
void rm_ext4(ext4_t *file);
void rm_ntfs(ntfs_t *file);

#endif