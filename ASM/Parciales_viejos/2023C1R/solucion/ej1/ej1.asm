global acumuladoPorCliente_asm
global en_blacklist_asm
global blacklistComercios_asm

extern calloc
extern malloc
extern strcmp

;########### SECCION DE TEXTO (PROGRAMA)
section .text

; uint32_t* acumuladoPorCliente(uint8_t cantidadDePagos, pago_t* arr_pagos)
; dil (rdi)->cantidadDePagos
; rsi -> arr_pagos
acumuladoPorCliente_asm:
	prologo:
		push rbp
		mov rbp, rsp
		push r12
		push r13

		xor r12, r12
		mov r12b, dil
		mov r13, rsi

		mov rdi, 10
		mov rsi, 4
		call calloc

		xor r10, r10
		xor r11, r11
		xor rcx, rcx

	.ciclo:
		mov r10b, byte [r13 + 17]
		cmp r10, 1
		jne .siguiente_pago


		mov r11b, byte [r13 + 16]
		mov cl, byte [r13 + 0]
		add dword [rax + r11*4], ecx

	.siguiente_pago:
		add r13, 24
		dec r12
		cmp r12, 0
		jne .ciclo

	.epilogo:
		pop r13
		pop r12	
		pop rbp
		ret

;uint8_t en_blacklist(char* comercio, char** lista_comercios, uint8_t n)
;rdi->comercio
;rsi->lista_comercios
;dl->n
en_blacklist_asm:
	;prologo
		push rbp
		mov rbp, rsp
		push r12
		push r13
		push r14
		push r15

		xor r14, r14

		mov r12, rdi ;comercio
		mov r13, rsi ;lista_comercios
		mov r14b, dl ;n

	.ciclo_comercios:
		mov r15, [r13] ;r15 puntero a string

		mov rdi, r12
		mov rsi, r15
		call strcmp
		cmp rax, 0
		je .epilogo

		add r13, 8 
		dec r14
		cmp r14, 0
		jne .ciclo_comercios

	.epilogo:
		pop r15
		pop r14
		pop r13
		pop r12
		pop rbp
		ret
;pago_t** blacklistComercios(uint8_t cantidad_pagos, pago_t* arr_pagos, char** arr_comercios, uint8_t size_comercios)
;dil->ctad pagos
;rsi->arr_pagos
;rdx->arr_comercios
;cl->size_comercios
blacklistComercios_asm:
	;prologo:
		push rbp
		mov rbp, rsp
		push r15
		push r14
		push r13
		push r12
		push rbx
		sub rsp, 8

		xor r12, r12
		xor r15, r15

		mov r12b, dil ;ctad_pagos
		mov r13, rsi ;arr_pagos
		mov r14, rdx ;arr_comercios
		mov r15b, cl ;size_comercios

		xor rbx, rbx ;pagos_en_comercios = 0
		mov r8, r12 ;i = ctad_pagos;
		mov r9, r13 ;puntero arr pagos para avanzar

	.ciclo_contador_de_pagos_en_comercios:
		mov rdi, [r9 + 8] ;paso primer param: arr_pagos[i].comercio
		mov rsi, r14 ;segundo param: arr_comercios
		mov rdx, r15 ;tercer páram

		push r8
		push r9

		call en_blacklist_asm

		pop r9
		pop r8

		cmp al, 0
		je .no_esta_en_blacklist1

		inc rbx ;pagos_en_comercios++

	.no_esta_en_blacklist1:
		dec r8 ;i++ (en realidad es lo opuesto pero se entiende)
		add r9, 24 ;avanzo al siguiente pago: arr_pagos[i++]
		cmp r8, 0
		jne .ciclo_contador_de_pagos_en_comercios

	.llamado_al_malloc:
		imul rbx, 8 ;rbx = sizeof(pago_t*) * pagos_en_comercios
		mov rdi, rbx 
		call malloc 
		mov rbx, rax ;me guardo la respuesta en rbx

		xor r10, r10 ;j = 0

		mov r8, r12 ;i = ctad_pagos;
		mov r9, r13 ;puntero arr pagos para avanzar
		 
	.ciclo2:
		mov rdi, [r9 + 8] ;paso primer param: arr_pagos[i].comercio
		mov rsi, r14 ;segundo param: arr_comercios
		mov rdx, r15 ;tercer páram

		push r8
		push r9
		push r10
		sub rsp, 8
		call en_blacklist_asm
		add rsp,8
		pop r10
		pop r9
		pop r8

		cmp al, 1
		jne .no_esta_en_blacklist2

		mov rdi, 24

		push r8
		push r9
		push r10
		sub rsp, 8
		call malloc
		add rsp,8
		pop r10
		pop r9
		pop r8

		mov r11b, byte [r9 + 0]
		mov byte [rax + 0], r11b ;copio monto

		mov r11, [r9 + 8]
		mov [rax + 8], r11 ;copio comercio

		mov r11b, byte [r9 + 16]
		mov byte [rax + 16], r11b ;copio cliente

		mov r11b, byte [r9 + 17]
		mov byte [rax + 17], r11b ;copio aprobado

		mov [rbx + r10], rax ;copio la direccion en el array respuesta (es un array de punteros)
		add r10, 8

	.no_esta_en_blacklist2:
		dec r8 ;i++ (en realidad es lo opuesto pero se entiende)
		add r9, 24 ;avanzo al siguiente pago: arr_pagos[i++]
		cmp r8, 0
		jne .ciclo2

	;epilogo:
		mov rax, rbx ;pongo la rta en rax
		add rsp, 8
		pop rbx
		pop r12
		pop r13
		pop r14
		pop r15
		pop rbp
