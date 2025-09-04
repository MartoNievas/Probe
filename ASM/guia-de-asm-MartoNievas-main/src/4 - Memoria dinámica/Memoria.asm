extern malloc
extern free
extern fprintf

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
	
	cmp al,0
	je fin_cadena1 ;termino la cadena 1

	cmp r8b,0
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
	cmp r8b, 0
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

; char* strClone(char* a)
strClone:
	ret

; void strDelete(char* a)
strDelete:
	ret

; void strPrint(char* a, FILE* pFile)
strPrint:
	ret

; uint32_t strLen(char* a)
strLen:
	ret


