

;########### ESTOS SON LOS OFFSETS Y TAMAÑO DE LOS STRUCTS
; Completar las definiciones (serán revisadas por ABI enforcer):
NODO_OFFSET_NEXT EQU 0
NODO_OFFSET_CATEGORIA EQU 8
NODO_OFFSET_ARREGLO EQU 16
NODO_OFFSET_LONGITUD EQU 24
NODO_SIZE EQU 32
PACKED_NODO_OFFSET_NEXT EQU 0
PACKED_NODO_OFFSET_CATEGORIA EQU 8
PACKED_NODO_OFFSET_ARREGLO EQU 9
PACKED_NODO_OFFSET_LONGITUD EQU 17
PACKED_NODO_SIZE EQU 21
LISTA_OFFSET_HEAD EQU 0
LISTA_SIZE EQU 8
PACKED_LISTA_OFFSET_HEAD EQU 0
PACKED_LISTA_SIZE EQU 8

NULL_POINTER EQU 0

;########### SECCION DE DATOS
section .data

;########### SECCION DE TEXTO (PROGRAMA)
section .text

;########### LISTA DE FUNCIONES EXPORTADAS
global cantidad_total_de_elementos
global cantidad_total_de_elementos_packed

;########### DEFINICION DE FUNCIONES
;extern uint32_t cantidad_total_de_elementos(lista_t* lista);
;registros: lista[RDI]
cantidad_total_de_elementos:
	;Prologo 
	push rbp
	mov rbp,rsp

	xor rax, rax ; inicializamos el contador en 0
	mov rsi, [rdi + LISTA_OFFSET_HEAD] ; rsi = head

	.ciclo	

	mov rcx, [rsi + NODO_OFFSET_LONGITUD] ;obtenemos la longitud
	add rax,rcx ;sumamos la longitud del nodo
	mov rsi,[rsi] ; rsi = nodo->next



	CMP rsi,NULL_POINTER ;
	jnz .ciclo ;Mientras rsi no sea 0 osea null seguimos el ciclo



	;epilogo
	pop rbp

	ret

;extern uint32_t cantidad_total_de_elementos_packed(packed_lista_t* lista);
;registros: lista[rdi]
cantidad_total_de_elementos_packed:
	;prologo
	push rbp 
	mov rbp,rsp

	xor rax, rax ;inicializamos el contador en 0 
	mov rsi, [rdi + PACKED_LISTA_OFFSET_HEAD] ;rsi = list->head

	.ciclo

		mov rcx, [rsi + PACKED_NODO_OFFSET_LONGITUD] ; cargo la longitud 
		add rax,rcx ; la sumo al contado
		mov rsi, [rsi + PACKED_NODO_OFFSET_NEXT] ;voy al siguiente nodo

		CMP rsi, NULL_POINTER
		jnz .ciclo

	;epilogo
	pop rbp
	ret

