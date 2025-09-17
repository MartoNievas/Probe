#include "ej1.h"

uint32_t* acumuladoPorCliente(uint8_t cantidadDePagos, pago_t* arr_pagos){
    uint32_t* arr_monto_total = calloc(10,sizeof(uint32_t) );
    //calloc xq inicializa en 0 y 10 porq son la cantidad de clientes

    for(uint8_t i = 0; i < cantidadDePagos; i++){
        if(arr_pagos[i].aprobado!=0){
            uint8_t cliente = arr_pagos[i].cliente;
            arr_monto_total[cliente] += arr_pagos[i].monto;
        }
    }
    return arr_monto_total;
}

uint8_t en_blacklist(char* comercio, char** lista_comercios, uint8_t n){
    for(uint8_t i = 0; i < n; i++){
        if(strcmp(comercio, lista_comercios[i])==0){
            return 1; // Lo encontré: está en la blacklist
        }
    }
    return 0; // No lo encontré: no está en la blacklist
}


pago_t** blacklistComercios(uint8_t cantidad_pagos, pago_t* arr_pagos, char** arr_comercios, uint8_t size_comercios){
    int pagos_en_comercios = 0;
    for(int i = 0; i < cantidad_pagos;i++){
        if(en_blacklist(arr_pagos[i].comercio, arr_comercios, size_comercios)){
            pagos_en_comercios += 1;
        }
    }
    pago_t** res = malloc(sizeof(pago_t*) * pagos_en_comercios);
    int j = 0;
    for(int i = 0; i < cantidad_pagos;i++){
        if(en_blacklist(arr_pagos[i].comercio, arr_comercios, size_comercios)){
            res[j] = &arr_pagos[i];
            j++;
        }

    }
    return res;
}


