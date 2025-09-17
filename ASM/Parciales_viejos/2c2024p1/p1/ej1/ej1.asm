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
EJERCICIO_1B_HECHO: db TRUE ; Cambiar por `TRUE` para correr los tests.

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

global es_indice_ordenado
; r/rdi = item_t**     inventario
; r/rsi = uint16_t*    indice
; r/rdx (dx) = uint16_t     tamanio
; r/rcx (f) = comparador_t comparador
es_indice_ordenado:
		;prologo:
			push rbp
			mov rbp, rsp
			push r15
			push r14
			push r13
			push r12 ;pusheo no volatiles para tener regs de sobra (queda alineada la pila)

			xor r10, r10 ;i=0
			xor r11, r11

			movzx rdx, dx ;extiendo dx a 8bytes
			dec rdx ;tamanio-1

		.ciclo:
			cmp r10, rdx ;i<?tamanio
			je .epilogo

		;if:
			movzx r15, word [rsi + r10*2]; r15 = indice[i] (cada dato son numeros de 2 bytes )
			mov r14, [rdi + r15*8]; r14 = inventario[indice[i]] = a (cada dato es de 8 son punteros)

			movzx r13, word [rsi + (r10 + 1) *2 ] ; r13 = indice[i+1]
			mov r12, [rdi + r13*8]; r12 = inventario[indice[i+1]] = b

			push rdi
			push rsi ;guardo los params orginales porq los voy a pisar (la pila sigue alineada)
			push rdx
			push rcx
			push r10 ;guardo mi contador porq es volatil
			push r11 ;alineo pila

			mov rdi, r14 ;paso los parametros
			mov rsi, r12
			call rcx ;comparador(a,b)

			pop r11
			pop r10
			pop rcx
			pop rdx
			pop rsi
			pop rdi ;restauro pila

			inc r10 ;i++

			cmp rax, 0 ;if(!comparador(a,b)) return false
			jne .ciclo 

		.epilogo:
			pop r12
			pop r13
			pop r14
			pop r15
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
; r/rdi = item_t**  inventario
; r/rsi = uint16_t* indice
; r/dx (rdx) = uint16_t  tamanio
indice_a_inventario:
	;prologo:
		push rbp
		mov rbp, rsp
		push r15
		push r14
		push r13
		push r12 ;pusheo no volatiles para tener regs de sobra (queda alineada la pila)

	;crear_nuevo_inventario:
		movzx r15, word dx ;extiendo a 8bytes dx
		;imul r15, 8 ;multiplico por sizeof(item_t*) que es 8 porq es puntero
		shl r15, 3  ;r15 = 8*tamanio

		push rdi ;preservo los params originales antes del call
		push rsi
		push rdx
		push rcx ;alineo pila

		mov rdi, r15 ;paso el parametro para malloc
		call malloc 

		pop rcx
		pop rdx
		pop rsi
		pop rdi ;restaurlo pila y params originales
		;en rax tengo el puntero a mi nuevo array (lo que tengo q devolver)

		;mov r14, rax ;me guardo una copia del puntero en r14
		xor r13, r13 ;i==0
		movzx r12, word dx ;r12=tamanio extendido a 8bytes

		xor r15, r15 ;limpio para volver a usar

	.ciclo:
		cmp r13, r12; i<? tamaio
		je .epilogo

		movzx r15, word [rsi + r13*2]; r15 = indice[i] (cada dato son numeros de 2 bytes)
		mov rcx, [rdi + r15*8]; rcx = inventario[indice[i]] = a (cada dato es de 8 son punteros)
		mov [rax + r13*8], rcx

		inc r13
		jmp .ciclo

	.epilogo:
		pop r12
		pop r13
		pop r14
		pop r15
		pop rbp
		ret
