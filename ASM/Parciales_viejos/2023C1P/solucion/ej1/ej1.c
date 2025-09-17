#include "ej1.h"

uint32_t cuantosTemplosClasicos_c(templo *temploArr, size_t temploArr_len){
    uint32_t contador_de_clasicos = 0;
    for(size_t i = 0; i < temploArr_len; i++){
        uint8_t c_larga_actual = temploArr[i].colum_largo;
        uint8_t c_corto_actual = temploArr[i].colum_corto;
        if(2*c_corto_actual + 1 == c_larga_actual){
            contador_de_clasicos += 1;
        }
    }
    return contador_de_clasicos;
}
  
templo* templosClasicos_c(templo *temploArr, size_t temploArr_len){
    uint32_t clasicos = cuantosTemplosClasicos(temploArr, temploArr_len);
    templo* templos_clasicos_arr = malloc(sizeof(templo) * clasicos);
    size_t j = 0;
    for(size_t i = 0; i < temploArr_len; i++){
        uint8_t c_larga_actual = temploArr[i].colum_largo;
        uint8_t c_corto_actual = temploArr[i].colum_corto;
        if(2*c_corto_actual + 1 == c_larga_actual){
            templos_clasicos_arr[j] = temploArr[i];
            j++;
        }
    }
    return templos_clasicos_arr;
}   

