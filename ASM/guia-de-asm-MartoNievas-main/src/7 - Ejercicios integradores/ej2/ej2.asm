extern malloc
extern free 
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
;   - optimizar
global EJERCICIO_2A_HECHO
EJERCICIO_2A_HECHO: db TRUE ; Cambiar por `TRUE` para correr los tests.

; Marca el ejercicio 1B como hecho (`true`) o pendiente (`false`).
;
; Funciones a implementar:
;   - contarCombustibleAsignado
global EJERCICIO_2B_HECHO
EJERCICIO_2B_HECHO: db FALSE ; Cambiar por `TRUE` para correr los tests.

; Marca el ejercicio 1C como hecho (`true`) o pendiente (`false`).
;
; Funciones a implementar:
;   - modificarUnidad
global EJERCICIO_2C_HECHO
EJERCICIO_2C_HECHO: db FALSE ; Cambiar por `TRUE` para correr los tests.

;########### ESTOS SON LOS OFFSETS Y TAMAÑO DE LOS STRUCTS
; Completar las definiciones (serán revisadas por ABI enforcer):
ATTACKUNIT_CLASE EQU 0
ATTACKUNIT_COMBUSTIBLE EQU 12
ATTACKUNIT_REFERENCES EQU 14
ATTACKUNIT_SIZE EQU 16

global optimizar

	; r/m64 = mapa_t           mapa -> rdi
	; r/m64 = attackunit_t*    compartida -> rsi 
	; r/m64 = uint32_t*        fun_hash(attackunit_t*) -> rdx
optimizar:
	push rbp ;alineada
	mov rbp,rsp
	push rbx ;desalineada
	push r12 ;alineada
	push r13 ;desalineada
	push r14 ;alineada 
	push r15 ;desalineada
	sub rsp,8 ;alineada 

	;primero verificamos que los punteros no sean nulos 
	cmp rdi,0
	je epilogo
	cmp rsi,0
	je epilogo
	cmp rdx,0
	je epilogo
	;Caso contrario movemos todos a registros no volalites 
	mov rbx,rdi ;pasamos el puntero del mapa a rbx 
	mov r12,rsi ;pasamos el puntero de la unidad compartidaa r12 
	mov r13,rdx ;pasamos el puntero a la funcion de hash a r13 

	;vamos a guardar el hash de compartido para tenerlo a mano 
	mov rdi,rsi
	call r13 
	mov rcx, rax ;ahora en rcx tenemos el hash de compartida 

	xor r14,r14 ;int i = 0

	ciclo_fila:
		xor r15,r15 ; j = 0; se reinicia cada vez 
		ciclo_columna:  

		mov r10,r14 ;r10 = i
		shl r10,8 ; r10 = i*256
		sub r10,r14 ;r10 = i*255 

		add r10,r15 ; r10 = i*255 + j 
		
		mov rdi,[rbx + r10*8] ;mapa[i][j]

		cmp rdi,0 ;verficamos si es un puntero nulo 
		je siguiente ;si lo es vamos con el siguiente elemento 

		;caso contrario verficamos que no sea el mismo puntero que compartida
		cmp rdi,r12 ;si es igual saltamos a siguiente 
		je siguiente 


		;por ultimo verificamos que tengan el mismo hash, vamos a verificar la condicion negada 
		push rdi 
		push r10  
		push rcx 
		sub rsp,8
		call r13 ; en rax tenemos el hash 
		;restauramos registr volatiles 
		add rsp,8
		pop rcx 
		pop r10 
		pop rdi ;restauro el valor de el elemento mapa[i][j]

		cmp rax, rcx ;si no coincide el hash 
		jne siguiente ;saltamos al siguiente 

		;caso contrario verifique todas las condiciones 

		dec dword [rdi + ATTACKUNIT_REFERENCES] ;decrementamos las referencias de atackunit 
		jnz no_liberar ;si las referencias no son 0, no liberamos memoria 

		;caso contrario si liberamos 
		push r10
		push rcx  
		call free ;en rdi esta la posicion de memoria a liberar 
		pop rcx 
		pop r10

		no_liberar: 
			mov [rbx + r10*8 ],r12 ;mapa[i][j] = compartido
			inc dword [r12 + ATTACKUNIT_REFERENCES] ;incrementamos las referencias 
 

		siguiente: 
			inc r15 ;incremento j++
			cmp r15,255 ;verifico si me quedan elemento
			jl ciclo_columna ;sigo con el siguiente elemento de la fila 

			;caso contrario termine de interar en esa fila 
			inc r14 ;i++ 
			cmp r14,255 ;comparo i < 255 
			jl ciclo_fila

			;si es mayor que fila y columna termine de iterar la matriz 

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

global contarCombustibleAsignado
contarCombustibleAsignado:
	; r/m64 = mapa_t           mapa
	; r/m64 = uint16_t*        fun_combustible(char*)
	ret

global modificarUnidad
modificarUnidad:
	; r/m64 = mapa_t           mapa
	; r/m8  = uint8_t          x
	; r/m8  = uint8_t          y
	; r/m64 = void*            fun_modificar(attackunit_t*)
	ret
