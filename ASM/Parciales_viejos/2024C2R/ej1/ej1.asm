extern malloc
extern free

section .rodata
; Acá se pueden poner todas las máscaras y datos que necesiten para el ejercicio

section .text
; Marca un ejercicio como aún no completado (esto hace que no corran sus tests)
FALSE EQU 0
; Marca un ejercicio como hecho
TRUE  EQU 1

;declaro cantidad del mapa
FILAS EQU 255
COLUMNAS EQU 255


; Marca el ejercicio 1A como hecho (`true`) o pendiente (`false`).
;
; Funciones a implementar:
;   - optimizar
global EJERCICIO_1A_HECHO
EJERCICIO_1A_HECHO: db TRUE ; Cambiar por `TRUE` para correr los tests.

; Marca el ejercicio 1B como hecho (`true`) o pendiente (`false`).
;
; Funciones a implementar:
;   - contarCombustibleAsignado
global EJERCICIO_1B_HECHO
EJERCICIO_1B_HECHO: db TRUE ; Cambiar por `TRUE` para correr los tests.

; Marca el ejercicio 1C como hecho (`true`) o pendiente (`false`).
;
; Funciones a implementar:
;   - modificarUnidad
global EJERCICIO_1C_HECHO
EJERCICIO_1C_HECHO: db TRUE ; Cambiar por `TRUE` para correr los tests.

;########### ESTOS SON LOS OFFSETS Y TAMAÑO DE LOS STRUCTS
; Completar las definiciones (serán revisadas por ABI enforcer):
ATTACKUNIT_CLASE EQU 0
ATTACKUNIT_COMBUSTIBLE EQU 12
ATTACKUNIT_REFERENCES EQU 14
ATTACKUNIT_SIZE EQU 16

global optimizar

; r/rdi = mapa_t           mapa
; r/rsi = attackunit_t*    compartida
; r/rdx = uint32_t*        fun_hash(attackunit_t*)

optimizar:
	;prologo:
		push rbp
		mov rbp, rsp
		push r12
		push r13
		push r14
		push r15 
		push rbx 
		sub rsp, 8 ;pusheo los callee save y alineo pila

	.preciclo:

		mov r12, rdi ;mapa
		mov r13, rsi ;compartida
		mov r14, rdx ;fun_hash
		
		mov rdi, r13 ;paso compartida como parametro
		call r14 ;llamo a fun hash
		mov ebx, eax ;ebx = fun_hash(compartida)
		
		xor r15, r15 ; i = 0

	.ciclo:
		mov rdi, [r12 + r15*8] ;rdi = mapa[i] (actual)
		cmp rdi, 0 ;mapa[i] ==? null
		je .siguiente_casillero
		cmp rdi, r13 ;mapa[i] == compartida?
		je .siguiente_casillero

		call r14 ;en rdi ya tengo actual
		cmp eax, ebx ;hash_actual ==? hash_compartida
		jne .siguiente_casillero

		inc BYTE [r13 + ATTACKUNIT_REFERENCES] ;compartida->references++
		mov rdi, [r12 + r15*8] ;rdi = mapa[i]
		dec BYTE [rdi + ATTACKUNIT_REFERENCES] ;mapa[i]->references--

		mov [r12 + r15*8], r13 ;mapa[i] = compartida

		cmp BYTE [rdi + ATTACKUNIT_REFERENCES], 0
		jne .siguiente_casillero
		call free ;ya esta en rdi el ptr a la unidad
		
	.siguiente_casillero:
		inc r15 ;i++
		cmp r15, FILAS * COLUMNAS ;i==?255*255
		jl .ciclo
	
	;epilogo:
		add rsp, 8
		pop rbx
		pop r15
		pop r14
		pop r13
		pop r12
		pop rbp
		ret

global contarCombustibleAsignado

; r/rdi = mapa_t           mapa
; r/rsi = uint16_t*        fun_combustible(char*)

contarCombustibleAsignado:

		push rbp
		mov rbp, rsp
		push r12
		push r13
		push r14
		push r15
		push rbx
		sub rsp, 8 ; pila alineada

		mov r15, rdi ; mapa
		mov r14, rsi ; fun_combustible
		xor r13, r13 ; total_combustible_utilizado
		xor r12, r12 ; iterador

	.loop:
		mov rsi, [r15 + r12*8] ; unidad actual
		cmp rsi, 0 ; ¿Es un null pointer?
		je .nextIteration

		movzx ebx, WORD [rsi + ATTACKUNIT_COMBUSTIBLE] ; actual->combustible

		add rsi, ATTACKUNIT_CLASE ; este add no es realmente necesario, el offset de clase es 0
		mov rdi, rsi; el puntero a donde comienza el string actual->clase (la función toma un char*)
		call r14
		movzx eax, ax ; combustible_base

		sub ebx, eax ; combustible_utilizado = actual->combustible - combustible_base
		add r13d, ebx ; total_combustible_utilizado += combustible_utilizado

	.nextIteration:
		inc r12
		cmp r12, FILAS * COLUMNAS
		jl .loop

		mov rax, r13

		add rsp, 8
		pop rbx
		pop r15
		pop r14
		pop r13
		pop r12
		pop rbp
		ret


global modificarUnidad
; r/rdi = mapa_t           mapa
; r/sil  = uint8_t          x
; r/dl  = uint8_t          y
; r/rcx = void*            fun_modificar(attackunit_t*)
modificarUnidad:
	;prologo:
		push rbp
		mov rbp, rsp
		push r12
		push r13
		push r14
		push r15
		push rbx
		sub rsp, 8 ; pila alineada

	;main:
		movzx rsi, sil ;extiendo x a 64
		movzx rdx, dl ;extiendo y a 64

		imul rsi, COLUMNAS ;rsi = x * columnas 
		add rdx, rsi ; rdx = y + x * columnas
		shl rdx, 3 ;rdx = (y + x * columnas) * 8
		add rdi, rdx ;rdi = mapa[x][y] (obtengo el casillero a modificar)

		mov r15, rdi ;r15 = mapa[x][y]
		mov r14, rcx ;r14 = f_modificar

		mov r12, [r15] ;unidad a modificar
		cmp r12, 0 ;unidad ==? null
		je .epilogo

		mov r13b, [r12 + ATTACKUNIT_REFERENCES] ;r13b = unidad->references
		cmp r13b, 1 ;unidad->references >? 1
		jle .no_fue_optimizado

		dec BYTE [r12 + ATTACKUNIT_REFERENCES] ;unidad->references-- porq le voy a hacer una copia

		mov rdi, ATTACKUNIT_SIZE
		call malloc ;pido espacio para la copia

		mov rdi, [r12]  ;copiamos contenido (struct entera)
		mov [rax], rdi 
		mov rdi, [r12 + 8]
		mov [rax + 8], rdi

		mov [rax + ATTACKUNIT_REFERENCES], BYTE 1 ;lan nueva es unica
		mov [r15], rax ;actualizamos el puntero del mapa

	.no_fue_optimizado:
		mov rdi, [r15]
		call r14 

	.epilogo:
		add rsp, 8
		pop rbx
		pop r15
		pop r14
		pop r13
		pop r12
		pop rbp
		ret
