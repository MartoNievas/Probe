extern malloc

section .rodata
; Acá se pueden poner todas las máscaras y datos que necesiten para el ejercicio

section .text
; Marca un ejercicio como aún no completado (esto hace que no corran sus tests)
FALSE EQU 0
; Marca un ejercicio como hecho
TRUE  EQU 1

; Marca el ejercicio 1A como hecho (`true`) o pendiente (`false`).
;
; Funciones a implementar:
;   - es_indice_ordenado
global EJERCICIO_1A_HECHO
EJERCICIO_1A_HECHO: db TRUE ; Cambiar por `TRUE` para correr los tests.

; Marca el ejercicio 1B como hecho (`true`) o pendiente (`false`).
;
; Funciones a implementar:
;   - indice_a_inventario
global EJERCICIO_1B_HECHO
EJERCICIO_1B_HECHO: db FALSE ; Cambiar por `TRUE` para correr los tests.

;########### ESTOS SON LOS OFFSETS Y TAMAÑO DE LOS STRUCTS
; Completar las definiciones (serán revisadas por ABI enforcer):
ITEM_NOMBRE EQU 0
ITEM_FUERZA EQU 20
ITEM_DURABILIDAD EQU 24
ITEM_SIZE EQU 28

;; La funcion debe verificar si una vista del inventario está correctamente 
;; ordenada de acuerdo a un criterio (comparador)

;; bool es_indice_ordenado(item_t** inventario, uint16_t* indice, uint16_t tamanio, comparador_t comparador);

;; Dónde:
;; - `inventario`: Un array de punteros a ítems que representa el inventario a
;;   procesar.
;; - `indice`: El arreglo de índices en el inventario que representa la vista.
;; - `tamanio`: El tamaño del inventario (y de la vista).
;; - `comparador`: La función de comparación que a utilizar para verificar el
;;   orden.
;; 
;; Tenga en consideración:
;; - `tamanio` es un valor de 16 bits. La parte alta del registro en dónde viene
;;   como parámetro podría tener basura.
;; - `comparador` es una dirección de memoria a la que se debe saltar (vía `jmp` o
;;   `call`) para comenzar la ejecución de la subrutina en cuestión.
;; - Los tamaños de los arrays `inventario` e `indice` son ambos `tamanio`.
;; - `false` es el valor `0` y `true` es todo valor distinto de `0`.
;; - Importa que los ítems estén ordenados según el comparador. No hay necesidad
;;   de verificar que el orden sea estable.

	; r/m64 = item_t**     inventario RDI (puntero)
	; r/m64 = uint16_t*    indice	  RSI (puntero)	
	; r/m16 = uint16_t     tamanio    RDX (numero de 16 bits)
	; r/m64 = comparador_t comparador RCX (puntero a funcion)

global es_indice_ordenado
es_indice_ordenado:
	push rbp ;alineada
	mov rbp,rsp
	push rdi ;desalineada
	push rsi ;alineada
	push rdx ;desalineada
	push rcx ;alineada
	push rbx ;desalineada
	push r8 ;alineada

	cmp rdi,0 ;Aqui verifico que no sean punteros nulos 
	je null_pointer
	cmp rsi, 0 ;aqui verficio que no sean punteros nulos
	je null_pointer

	cmp rdx,1 ;Aqui verficio el caso donde tengo 1 o 0 indices entonces ya esta ordenado
	jle ordenada

	;Ahora vamos a reccorrer el inventario con un ciclo 
	xor r10,r10 ;int i = 0
	mov rbx,rdi ; movemos rdi a rbx, que es el puntero base a inventario
	mov r8,rsi	; movemos rsi a r8, que es el puntero base a indices 
	sub rdx,1 ;tamanio = tamanio - 1, esto para ir recorriendo los indices de 2 en 2 
	
	ciclo: ;ciclo para la verificacion
	cmp r10,rdx ;aqui comparo que si i >= tamnio - 1 entonces ya termine de recorrer por lo tanto esta ordenada
	jge ordenada ;i >= tamanio -1

	movzx r12,  word[r8 + r10*2] ;indice[i]

	movzx r13, 	word [r8 + r10*2 + 2] ;indice[i+1]


	mov rdi, [rbx + r12*8] ;inventario[indice[i]]
	mov rsi, [rbx + r13*8] ;inventario[indice[i+1]]

	test rdi,rdi ;comparamos que no sean punteros nulos 
	jz null_pointer
	test rsi,rsi
	jz null_pointer

	;ahora llamamos a la funcion de comparacion para eso queremos preservar los indices r10 y r11

	;Con el push guardo el inidice en el que me encuentro en la pila por si la funcion
	;comparador lo modifica 
	push r10 ;aqui desalineo la pila 
	sub rsp,8 ;mantengo la alineacion de la pila 
	call rcx
	add rsp,8 ;elimino el padding que inserte 
	pop r10 ;restauro el valor de r10 

	test rax,rax ;verficio el valor de la funcion comparador 
	jle no_ordenada ;si es FALSE == 0 entonces no esta ordenada

	; caso contrario continuo el ciclo
	inc r10 ;incremento el indice i++ 
	jmp ciclo ;salto incondicional al ciclo 


	ordenada:
	mov rax,TRUE
	jmp epilogo

	no_ordenada:
	mov rax,FALSE
	jmp epilogo

	null_pointer: 
	mov rax,FALSE

	epilogo:
		pop r8
		pop rbx 
		pop rcx 
		pop rdx 
		pop rsi
		pop rdi
		mov rsp,rbp
		pop rbp
		ret

;; Dado un inventario y una vista, crear un nuevo inventario que mantenga el
;; orden descrito por la misma.

;; La memoria a solicitar para el nuevo inventario debe poder ser liberada
;; utilizando `free(ptr)`.

;; item_t** indice_a_inventario(item_t** inventario, uint16_t* indice, uint16_t tamanio);

;; Donde:
;; - `inventario` un array de punteros a ítems que representa el inventario a
;;   procesar.
;; - `indice` es el arreglo de índices en el inventario que representa la vista
;;   que vamos a usar para reorganizar el inventario.
;; - `tamanio` es el tamaño del inventario.
;; 
;; Tenga en consideración:
;; - Tanto los elementos de `inventario` como los del resultado son punteros a
;;   `ítems`. Se pide *copiar* estos punteros, **no se deben crear ni clonar
;;   ítems**

global indice_a_inventario
indice_a_inventario:
	; Te recomendamos llenar una tablita acá con cada parámetro y su
	; ubicación según la convención de llamada. Prestá atención a qué
	; valores son de 64 bits y qué valores son de 32 bits o 8 bits.
	;
	; r/m64 = item_t**  inventario
	; r/m64 = uint16_t* indice
	; r/m16 = uint16_t  tamanio
	ret
