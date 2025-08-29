//*************************************
//Declaración de estructuras
//*************************************

// Lista de arreglos de enteros de 32 bits sin signo.
// next: Siguiente elemento de la lista o NULL si es el final
// categoria: Categoría del nodo
// arreglo: Arreglo de enteros
// longitud: Longitud del arreglo
typedef struct nodo_s {
    struct nodo_s* next;   //asmdef_offset:NODO_OFFSET_NEXT
    uint8_t categoria;     //asmdef_offset:NODO_OFFSET_CATEGORIA
    uint32_t* arreglo;     //asmdef_offset:NODO_OFFSET_ARREGLO
    uint32_t longitud;     //asmdef_offset:NODO_OFFSET_LONGITUD
} nodo_t; //asmdef_size:NODO_SIZE

typedef struct __attribute__((__packed__)) packed_nodo_s {
    struct packed_nodo_s* next;   //asmdef_offset:PACKED_NODO_OFFSET_NEXT
    uint8_t categoria;     //asmdef_offset:PACKED_NODO_OFFSET_CATEGORIA
    uint32_t* arreglo;     //asmdef_offset:PACKED_NODO_OFFSET_ARREGLO
    uint32_t longitud;     //asmdef_offset:PACKED_NODO_OFFSET_LONGITUD
} packed_nodo_t; //asmdef_size:PACKED_NODO_SIZE

// Puntero al primer nodo que encabeza la lista
typedef struct lista_s {
    nodo_t* head;    //asmdef_offset:LISTA_OFFSET_HEAD
} lista_t; //asmdef_size:LISTA_SIZE

// Puntero al primer nodo que encabeza la lista
typedef struct __attribute__((__packed__)) packed_lista_s {
    packed_nodo_t* head;    //asmdef_offset:PACKED_LISTA_OFFSET_HEAD
} packed_lista_t; //asmdef_size:PACKED_LISTA_SIZE
