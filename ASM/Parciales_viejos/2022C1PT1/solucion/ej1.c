#include "ejs.h"
#include "str.h"


void strArrayPrint(str_array_t* a, FILE* pFile) {
    fprintf(pFile, "[");
    for(int i=0; i<a->size-1; i++) {
        strPrint(a->data[i], pFile);
        fprintf(pFile, ",");
    }
    if(a->size >= 1) {
        strPrint(a->data[a->size-1], pFile);
    }
    fprintf(pFile, "]");
}

char* strArrayRemove(str_array_t* a, uint8_t i) {
    char* ret = 0;
    if(a->size > i) {
        ret = a->data[i];
        for(int k=i+1;k<a->size;k++) {
            a->data[k-1] = a->data[k];
        }
        a->size = a->size - 1;
    }
    return ret;
}

char* strArrayGet(str_array_t* a, uint8_t i) {
    char* ret = 0;
    if(a->size > i)
        ret = a->data[i];
    return ret;
}

// str_array_t* strArrayNew(uint8_t capacity){
//     str_array_t* new_str_arr = malloc(sizeof(str_array_t));
//     new_str_arr->size = 0;
//     new_str_arr->capacity = capacity;
//     //new_str_arr->data = malloc(sizeof(char*) * capacity);
//     new_str_arr->data = calloc(capacity, sizeof(char*)); //los inicializa en 0
//     //DATA ES UN ARRAY DE PUNTEROS A STRINGS (STRING = PUNTERO A CHAR)
//     return new_str_arr;
// }

// uint8_t  strArrayGetSize(str_array_t* a){
//     uint8_t ocupados = a->size;
//     return ocupados;
// }

// void  strArrayAddLast(str_array_t* a, char* data){
//     uint8_t ocupados = a->size;
//     char** strings = a->data;

//     if(ocupados == a->capacity){
//         return; //no hago nada
//     }
    
//     strings[ocupados] = strdup(data);
//     a->size++; //actualizo size
// }

// void  strArraySwap(str_array_t* a, uint8_t i, uint8_t j){
//     if(i >= a->size ||  j >= a->size){
//         return;
//     }
//     char* temp = a->data[i];
//     a->data[i] = a->data[j];
//     a->data[j] = temp;

// }

// void  strArrayDelete(str_array_t* a){
//     for(uint8_t i = 0; i< a->size; i++){ //hasta size o hasta capacity??
//         free(a->data[i]);
//     }
//     free(a->data);
//     free(a);
// }
// // IMPLEMENTACION PROPIA DE strdup si no está
// char* strdup(const char* s) {
//     size_t len = strlen(s) + 1;
//     char* new_str = malloc(len);
//     if (new_str) {
//         memcpy(new_str, s, len);
//     }
//     return new_str;
// }