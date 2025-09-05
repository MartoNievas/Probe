extern malloc
extern free
extern fprintf
NULL_POINTER EQU 0
section .data

section .text

global strCmp
global strClone
global strDelete
global strPrint
global strLen

; ** String **
;se nos pasan 2 punteros entonces RDI <- a y RSI <- b 
; int32_t strCmp(char* a, char* b)
strCmp:
	;prologo
	push rbp
	mov rbp,rsp

	;Ponemos el caracter inicial en registros volatiles 
	mov al, [rdi] ;r8 -> a
	mov r8b, [rsi] ;r9 -> b

	.ciclo
	
	cmp al,NULL_POINTER
	je fin_cadena1 ;termino la cadena 1

	cmp r8b,NULL_POINTER
	je fin_cadena2 ;termino la cadena 2 

	cmp al, r8b ;Comparamos ambos caracteres
	jne caracteres_diferentes

	
	inc rdi	;nos movemos al siguiente char de a 
	inc rsi ;nos movemos al siguiente char de b
	
	;actualizamos r8b y al 
	mov al,[rdi]
	mov r8b,[rsi]

	jmp .ciclo ;ciclo

	caracteres_diferentes

	;aqui decidimos que caracter es mayor 
	cmp al,r8b 
	js a_menor ;si la resta al - r8b < 0 quiere decir que r8b > al erntonces la palabra b > a 
	;Caso contrario
	mov eax,-1 ; como no se realizo el salto anterior quiere decir que a > b entonces devolver
	jmp epilogo


	a_menor
		mov eax, 1 ;como a < b asignamos 1
		jmp epilogo ; saltamos al epilogo


	fin_cadena1
	cmp r8b, NULL_POINTER
	je iguales ;como son iguales saltamos a iguales 

	;si no son iguales entonces a b le falta terminar por lo tanto es mas grande b > a
	mov eax,1
	jmp epilogo


	fin_cadena2
	;si llegamos aqui quiere decir que la cadena a es mas larga que b entonces, a > b devuelve -1 
	mov eax, -1 ;asinamos -1 al registro de salida 
	jmp epilogo ;saltamos al epilog


	iguales
		mov eax, 0 ;como son iguales entonces ponemos 0 en eax
		jmp epilogo ;saltamos al epilogo

	epilogo
	pop rbp
	ret

; char* strClone(char* a) el argumento se paso por rdi 
strClone:
	push rbp ;alineada 
	mov rbp,rsp

	push rdi ;desalineada 
	push rdx ;alineada
	push r9 ;desalineada
	push r8 ;alineda
	
	; primero verficamos que el puntero no sea null
	cmp rdi, 0
	je puntero_nulo

	xor r9,r9 ;contador de bytes 
	mov r9b,[rdi] ;primer caraccter 

	;ahora contamos la cantidad de chars + 1
	contar_bytes: 
	cmp r8b,0 ;verificamos que no se el char nulo
	je fin_conteo
	
	inc r9 ;contador++
	inc rsi ; nos movemos al siguiente char
	mov r8b, [rdi]
	jmp contar_bytes

	fin_conteo:
	inc r9 ;contador++ por el char nulo 

	mov rsi,r9 ;para llamar a malloc 
	call malloc ;puntero nuevo en rax
	mov rdx, rax ;movemos el puntero a rdx para poder incrementarlo

	cmp rdx,0 ;verificamos que se asgino la memoria
	je puntero_nulo ;si no se asgino devolvemos un puntero nulo

	mov rdi,[rbp - 8] ;recuperamos rdi de la pial

	mov r8b,[rdi]
	copiar_cadena:
    mov r8b, [rdi]      ; leer carácter
    mov [rdx], r8b      ; escribir en destino
    inc rdi             ; avanzar source
    inc rdx             ; avanzar destino
    cmp r8b, 0       ; verificar si es byte nulo
    jnz copiar_cadena   ; seguir si no es nulo

    jmp fin



	puntero_nulo:
	xor rax,rax
	jmp fin

	;epilogo
	fin:
	pop r8
	pop r9
	pop rdx
	pop rdi
	pop rbp
	mov rsp,rbp
	pop rbp
	ret

; void strDelete(char* a) puntero en rdi 
strDelete:
	;prolog
	push rbp
	mov rbp, rsp

	cmp rdi,0
	je fin1

	;en rdi tenemos el puntero a liberar 
	call free ;listo esto libera la memoria 

	fin1:
	pop rbp
	mov rsp,rbp

	ret

; void strPrint(char* a, FILE* pFile) a[rdi] y pFilep[rsi]
strPrint:
	push rbp
	mov rbp,rsp

	cmp rsi, 0 
	je fin2

	;cadena nula 
	cmp rdi,0
	je fin2

	call fprintf


	fin2:
	pop rbp
	mov rsp,rbp
	ret

; uint32_t strLen(char* a) puntero por rdi
strLen:
	;prologo 
	push rbp
	mov rbp,rsp
	push r8
	push rbx

	cmp rdi,0
	je .fin

	mov rbx,rdi
	xor eax,eax ;contador de chars 
	mov r8b, [rbx]
	
	.contador:
	cmp r8b,0
	je .epilogo 
	inc rbx
	inc eax 
	mov r8b,[rbx]

	jmp .contador

	.fin
		xor eax,eax
		jmp .epilogo

	.epilogo:
	pop rbx
	pop r8
	mov rsp,rbp
	pop rbp
	ret


