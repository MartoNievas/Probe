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
	push rbp ;stack alineado
	mov rbp,rsp ;ponemos el base poiner apuntando a la pila 
	push rbx ;desalineada 
	push r12 ;alineada 
	push r13 ;desalineada
	push r14 ;alineada 
	push r15 ;desalineda 
	sub rsp,8 ;alineada 

	;verificamos que los punteros no sean nulos 
	cmp rdi,0
	je no_ordenada
	cmp rsi,0
	je no_ordenada
	cmp rcx,0
	je no_ordenada

	cmp rdx, 1
	jle ordenada ; si tamanio <= 1 esta ordeanada 

	;ahora movemos los datos a los registros no volatiles 
	mov rbx,rdi ;movemos el puntero de inventario a rbx
	mov r12,rsi ;guardamos el puntero a los indices 
	mov r13, rdx ;guardamos el tamanio 
	mov r14, rcx ;el puntero a la funcion de comparacion 

	sub r13,1 ;ajustamos el tamanio = tamanio - 1
	xor r15,r15 ;int i = 0;
	
	ciclo:
	cmp r15,r13
	jge ordenada ;i >= tamanio - 1 entonces recorrimos toda la lista y esta ordenana 

	;buscamos los indices 
	movzx rdi, word [r12 + r15*2] ;rdi = indices[i]
	movzx rsi, word [r12 + r15*2 + 2] ; rsi = indices[i+1]

	;ahora buscamos los punteros de los items 
	mov rdi,[rbx + rdi*8]
	mov rsi,[rbx + rsi*8]

	;verificamos que no sean nulos
	cmp rdi,0
	je no_ordenada
	cmp rsi,0
	je no_ordenada

	;ahora llamamos a la funcion de comparacion que tiene los parametros en rdi y rsi respectivamente 
	call r14 
	;ahora tenemos el resultado en rax 
	cmp rax,FALSE ;significa que ese par no esta ordenado, por lo tanto la vista entera no lo esta 
	je no_ordenada ;saltamos a no ordenada 

	;caso contrario 
	inc r15
	jmp ciclo


	ordenada:
		mov rax, TRUE
		jmp epilogo

	no_ordenada: 
		mov rax,FALSE
	
	
	epilogo: 
	add rsp,8
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
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

; r/m64 = item_t**  rdi <- inventario puntero
	; r/m64 = uint16_t* rsi <- indice puntero 
	; r/m16 = uint16_t  rdx <- tamanio unsigned int de 16 bits (2 bytes)

global indice_a_inventario
indice_a_inventario:
		push rbp ;alineamos la pila
	mov rbp,rsp ;Movemos rsp al base pointer 
	push rbx ;desalineada 
	push r12 ;alineada 
	push r13 ;desalineada
	push r14 ;alineada 


	;verificamos que los punteros no sean nulos 
	;si alguno es nulo devolvemos el puntero nulo
	cmp rdi, 0 ;verificamos que el puntero inventario no sea null 
	je null_pointer 
	cmp rsi,0
	je null_pointer

	;caso contario 
	;movemos los datos de entrada a registros no volatiles 
	mov rbx,rdi ;puntero a inventario 
	mov r12,rsi ;puntero a los indices 
	mov r13,rdx ;tamanio 

	xor r14,r14 ;int i = 0; 
	;ahora tenemos que pedir la memoria con malloc, para eso debemos pasarle sizeof(item_t) * tamanio 
	mov rdi,r13 ;pasamos el tamanio a rdi 
	imul rdi,28 ;multiplicamos por el itemsize 

	call malloc ;llamamos a malloc 

	;verificamos que el puntero no sea nulo 
	cmp rax, 0 
	je epilogo1

	;caso contrario copiamos la vista del inventario 
	ciclo1:
		cmp r14,r13 ;si i >= tamanio ya recorrimos los indices 
		jge epilogo1

		;ahora busamos el inidice 
		movzx rdi, word [r12 + r14*2] ;indices[i]

		mov rdi, [rbx + rdi*8] ;inventario[indice[i]]

		mov [rax + r14*8], rdi ;guardamos el puntero en memoria 

		inc r14
		jmp ciclo1

	null_pointer:
		xor rax,rax 

	epilogo1:
	pop r14 
	pop r13
	pop r12 
	pop rbx
	mov rsp,rbp
	pop rbp
	ret
