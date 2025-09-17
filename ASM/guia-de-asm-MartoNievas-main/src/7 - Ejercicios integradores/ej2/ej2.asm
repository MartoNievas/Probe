extern malloc
extern free 
extern srtncpy
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
EJERCICIO_2B_HECHO: db TRUE ; Cambiar por `TRUE` para correr los tests.

; Marca el ejercicio 1C como hecho (`true`) o pendiente (`false`).
;
; Funciones a implementar:
;   - modificarUnidad
global EJERCICIO_2C_HECHO
EJERCICIO_2C_HECHO: db TRUE ; Cambiar por `TRUE` para correr los tests.

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

		dec byte [rdi + ATTACKUNIT_REFERENCES] ;decrementamos las referencias de atackunit 
		jnz no_liberar ;si las referencias no son 0, no liberamos memoria 

		;caso contrario si liberamos 
		push r10
		push rcx  
		call free ;en rdi esta la posicion de memoria a liberar 
		pop rcx 
		pop r10

		no_liberar: 
			mov [rbx + r10*8 ],r12 ;mapa[i][j] = compartido
			inc byte [r12 + ATTACKUNIT_REFERENCES] ;incrementamos las referencias 
 

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

; r/m64 = mapa_t           mapa -> rdi
	; r/m64 = uint16_t*        fun_combustible(char*) -> rsi
global contarCombustibleAsignado
contarCombustibleAsignado:
	push rbp ;alineado
	mov rbp,rsp 
	push rbx ;desalineada
	push r12 ;alineada
	push r13 ;desalineada
	push r14 ;alineada 
	push r15 ;desalineada
	sub rsp,8 ;alineada 

	;inicializamo res = -1 por si es null pointer algun parametro 
	mov rax,-1

	;veamos que los punteros no son nulos 
	cmp rdi,0 
	je .epilogo ; si lo son retorno -1 codigo de error 
	cmp rsi,0
	je .epilogo

	;caso contrario movemos los argumentos a los registros no volatiles 
	mov rbx,rdi ;pasamos el puntero al mapa a rbx
	mov r12,rsi ;pasamos el puntero a la funcion a r12 

	xor r13,r13 ;res = 0; 
	xor r14,r14 ;int i = 0 ;
	
	.ciclo_fila1: 
		xor r15,r15 ; int j = 0; 
		.ciclo_columna1: 

			mov r10,r14
			shl r10,8 ;r10 = i*256
			sub r10,r14 ;r10 = i*255
			add r10,r15 ;r10 = i*255 + j 
			
			;ahora busquemos el puntero de la unidad de atque 
			mov rdi,[rbx + r10*8] ;rdi = mapa[i][j]

			;verifiquemos que no es un puntero nulo 
			cmp rdi,0 ; si es nulo vamos con el siguiente
			je .siguiente

			;caso contrario 
			movzx eax, word [rdi + ATTACKUNIT_COMBUSTIBLE]
			add r13d, eax ;en r13w ponemos el res parcial 

			;ahora debemos restar el combustible base, para eso llamamos a la funcion fun_combustible, la cual toma como parametro
			;el nombre de la clase 

			;siempre que una funcion toma como parametro un puntero, y tenemos un tipo de dato que no es un puntero 
			;usamos lea para obtener la direccion de ese tipo Load Effective Address (lea)
			lea rdi,[rdi + ATTACKUNIT_CLASE];pasamo como parametro el nombre de la clase 
			call r12 ;resultado en rax 
			
			;restamos el combustible base
			sub r13d,eax 

			.siguiente:
				inc r15 ;j++
				cmp r15,255 
				jl .ciclo_columna1 ; i < 255 

				inc r14 ;i++
				cmp r14,255 
				jl .ciclo_fila1  ; j<255

				mov eax,r13d ;pasamos el res parcial a rax 


				;caso contrario termino
			.epilogo:
			add rsp,8 
			pop r15
			pop r14
			pop r13
			pop r12
			pop rbx 
			mov rsp,rbp 
			pop rbp
			ret

; r/m64 = mapa_t           rdi <- mapa 
	; r/m8  = uint8_t      rsi <- x
	; r/m8  = uint8_t      rdx <- y
	; r/m64 = void*        rcx <- fun_modificar(attackunit_t*)
global modificarUnidad
modificarUnidad:
	push rbp 
	mov rbp,rsp 
	push rbx 
	push r12 
	push r13 
	push r14
	push r15
	sub rsp,8
	;stack alineado 

	;verifiquemos que los punteros no son nulos y los indices son validos 
	cmp rdi,0
	je .epilogo
	cmp rcx,0
	je .epilogo
	cmp rsi,255 
	jge .epilogo
	cmp rdx, 255
	jge .epilogo 
	;guardamos el puntero a mapa y el puntero a la funcion en un registro no volatil 
	mov rbx,rdi 
	mov r15,rcx
	

	;caso contrario busamos mapa[x][y]

	;primero numero de fila 
	mov r12,rsi
	shl r12,8 ; r12 = 256*x 
	sub r12,rsi; r12 = 255*x

	;sumamos el numero de columna 
	add r12,rdx; r12 = 255*x + y 

	mov r13, [rbx + r12*8]; r13 = mapa[x][y] = modifier 

	;verificamos que el puntero no sea nulo 
	cmp r13,0
	je .epilogo 

	;caso contrario, vamos con la modificacion 
	;primer caso si hay mas de una referencia 
	movzx rdi, byte [r13 + ATTACKUNIT_REFERENCES]
	cmp rdi, 1 ;si tiene mas de una referencia creamos una nueva unidad pidiendo memoria 
	jg .nueva_unidad  

	;caso contrario modificamos la que ya existe 
	mov rdi,r13 ; rdi = mapa[x][y]
	jmp .modificarUnidad

	.nueva_unidad: 
		;ahora debemos copiar todos los atributos de modifier y pedir memoria 
		mov rdi, ATTACKUNIT_SIZE
		call malloc ; tenemos el puntero en rax 

		cmp rax,0 
		je .epilogo ;si el puntero es nulo saltamos al epilogo 

		.copiarUnidad:
		;caso contrario copiamos los datos 
		mov [rax + ATTACKUNIT_REFERENCES], byte 1 ; copiamos las referencias 
		
		;copaimos el combustible 
		mov r10w, word [r13 + ATTACKUNIT_COMBUSTIBLE]
		mov word [rax + ATTACKUNIT_COMBUSTIBLE], r10w
		;ahora debemos copiar el nombre de la clase  

		mov r10, [r13 + ATTACKUNIT_CLASE] ;primeros 8 bytes de clase 
		mov [rax + ATTACKUNIT_CLASE], r10
		mov r10d, [r13 + ATTACKUNIT_CLASE + 8]
		mov [rax + ATTACKUNIT_CLASE + 8],r10d

		.remplazarUnidad: 
			mov [rbx + r12*8], rax ;remplazamos la unidas por la copia con una sola referencia

		.disminuirReferenciasDeLaOriginal: 
			mov r10b, byte [r13 + ATTACKUNIT_REFERENCES]
			dec r10b
			mov byte [r13 + ATTACKUNIT_REFERENCES],r10b

			mov rdi,rax
		.modificarUnidad: 
			call r15
		
		.epilogo: 
			add rsp,8 
			pop r15
			pop r14
			pop r13
			pop r12
			pop rbx 
			mov rsp,rbp 
			pop rbp
	ret
