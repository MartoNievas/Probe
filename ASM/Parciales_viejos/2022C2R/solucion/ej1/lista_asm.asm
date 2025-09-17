; %define OFFSET_NEXT  ??
; %define OFFSET_SUM   ??
; %define OFFSET_SIZE  ??
; %define OFFSET_ARRAY ??

BITS 64
extern calloc
section .text


; uint32_t proyecto_mas_dificil(lista_t*)
;
; Dada una lista enlazada de proyectos devuelve el `sum` más grande de ésta.
;
; - El `sum` más grande de la lista vacía (`NULL`) es 0.
; lista rdi 
global proyecto_mas_dificil
proyecto_mas_dificil:
	push rbp 
	mov rbp, rsp 
	
	push r12
	push r13
	push r14
	push r15

	xor r12,r12 ;contador 
	xor r13, r13;maximo 
	xor r14, r14 ;lista

	mov r14, rdi ;lista 
.loop: 
	cmp r14, 0
	je .fin
	xor r15, r15
	mov r15d, [r14 + 8] ;sum
	cmp r15, r13 
	jnle .cuenta
	mov r14, [r14] 
	jmp .loop
.cuenta:
	xor r13,r13
	mov r13, r15
	mov r14, [r14] 
	jmp .loop


.fin:
	mov rax, r13
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbp
	ret

; void tarea_completada(lista_t*, size_t)
;
; Dada una lista enlazada de proyectos y un índice en ésta setea la i-ésima
; tarea en cero.
;
; - La implementación debe "saltearse" a los proyectos sin tareas
; - Se puede asumir que el índice siempre es válido
; - Se debe actualizar el `sum` del nodo actualizado de la lista
;rdi liata, index rsi
global marcar_tarea_completada
marcar_tarea_completada:
	push rbp 
	mov rbp, rsp 
	
	push r12
	push r13
	push r14
	push r15
	push rbx
	xor r12,r12 ;contador 
	xor r13, r13;index
	xor r14, r14 ;lista

	mov r14, rdi
	mov r13, rsi
.loop:
	cmp r14, 0
	je .fin
	xor r15, r15
	xor rbx, rbx 
	mov r15, [r14 + 16] ; size
	add rbx, r12
	add rbx, r15
	cmp rbx, r13
	jnle .fin 
	add r12, r15
	mov r14, [r14]
	jmp .loop 

.fin:
	cmp r14, 0
	je .fin
	sub r13, r12
	xor r15, r15
	mov r15, [r14 + 8] ; sum
	mov rdx, [r14 + 24] ; array
	mov rcx, [rdx + r13 * 4] ; array[index]
	sub r15, rcx
	mov [r14 + 8], r15
	mov word [rdx + r13 * 4], 0
	pop rbx
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbp
	ret

; uint64_t* tareas_completadas_por_proyecto(lista_t*)
;
; Dada una lista enlazada de proyectos se devuelve un array que cuenta
; cuántas tareas completadas tiene cada uno de ellos.
;
; - Si se provee a la lista vacía como parámetro (`NULL`) la respuesta puede
;   ser `NULL` o el resultado de `malloc(0)`
; - Los proyectos sin tareas tienen cero tareas completadas
; - Los proyectos sin tareas deben aparecer en el array resultante
; - Se provee una implementación esqueleto en C si se desea seguir el
;   esquema implementativo recomendado
;
global tareas_completadas_por_proyecto
tareas_completadas_por_proyecto:
	push rbp 
	mov rbp, rsp 
	
	push r12
	push r13
	push r14
	push r15
	push rbx
	push rdi
	push rsi
	push rcx  
	
	xor r12, r12;contador
	xor r13, r13;tamaño
	xor r14,r14;lista
	xor r15, r15;resultado 
	
	mov r14,rdi
	call lista_len
	mov r13, rax
	mov rbx, r13
	mov rdi, rbx
	mov rsi, 8
	call calloc
	mov r15, rax
.loop: 
	cmp r12, r13
	je .fin
	xor rdi, rdi
	xor rsi, rsi  
	xor rcx, rcx 
	mov rdi, [r14 + 24] ;array
	mov rsi, [r14 + 16] ;size 
	call tareas_completadas
	mov [r15 + r12 * 8],rax
	mov r14, [r14]
	inc r12
	jmp .loop 
.fin:
	xor rax, rax
	mov rax, r15
	pop rcx
	pop rsi
	pop rdi 
	pop rbx
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbp
	ret

; uint64_t lista_len(lista_t* lista)
;
; Dada una lista enlazada devuelve su longitud.
;
; - La longitud de `NULL` es 0
; rdi lista
lista_len:
	push rbp 
	mov rbp, rsp
	push r12
	push r13
	
	xor r13, r13
	xor r12, r12

	mov r12, rdi ;lista
.loop:
	cmp r12, 0
	je .fin 
	inc r13 
	mov r12, [r12]
	jmp .loop 

.fin:
	mov rax, r13
	pop r13
	pop r12
	pop rbp 
	ret

; uint64_t tareas_completadas(uint32_t* array, size_t size) {
;
; Dado un array de `size` enteros de 32 bits sin signo devuelve la cantidad de
; ceros en ese array.
;
; - Un array de tamaño 0 tiene 0 ceros.
tareas_completadas:
	push rbp
	mov rbp, rsp 
	push r12
	push r13
	push r14
	push r15
	push rbx
	push rdi 
	push rsi 
	push rcx 
	
	xor r12, r12;completadas
	xor r13, r13;contador
	xor r14, r14;lista
	xor r15, r15;tamaño
	mov r14, rdi
	mov r15, rsi 
	cmp r14, 0
	je .fin 
.loop:
	cmp r13, r15
	je .fin
	xor rbx, rbx 
	mov ebx, [r14 + r13 * 4]
	cmp ebx, 0
	je .cuenta 
	inc r13
	jmp .loop 
.cuenta:
	inc r12
	inc r13
	jmp .loop 
.fin:
	xor rax, rax
	mov rax, r12
	pop rcx 
	pop rsi 
	pop rdi
	pop rbx
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbp
	ret