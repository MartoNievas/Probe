//*************************************
//Declaración de estructuras
//*************************************

typedef struct item_s {
	char nombre[9]; 	//asmdef_offset:ITEM_OFFSET_NOMBRE
	uint32_t id;		//asmdef_offset:ITEM_OFFSET_ID
	uint32_t cantidad;	//asmdef_offset:ITEM_OFFSET_CANTIDAD
} item_t;