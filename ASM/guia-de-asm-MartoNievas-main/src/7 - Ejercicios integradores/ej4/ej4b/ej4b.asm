extern strcmp
global invocar_habilidad

; Completar las definiciones o borrarlas (en este ejercicio NO serán revisadas por el ABI enforcer)
DIRENTRY_NAME_OFFSET EQU 0
DIRENTRY_PTR_OFFSET EQU 16
DIRENTRY_SIZE EQU 24

FANTASTRUCO_DIR_OFFSET EQU 0
FANTASTRUCO_ENTRIES_OFFSET EQU 8
FANTASTRUCO_ARCHETYPE_OFFSET EQU 16
FANTASTRUCO_FACEUP_OFFSET EQU 24
FANTASTRUCO_SIZE EQU 32

section .rodata
; Acá se pueden poner todas las máscaras y datos que necesiten para el ejercicio

section .text

; void invocar_habilidad(void* carta[rdi], char* habilidad[rsi]);
invocar_habilidad:
	push rbp 
	mov rbp,rsp 
	push r15 
	push r14 
	push r13 
	push r12
	push rbx 
	sub rsp,8

	;verficamos que los punteros no sean nulos 
	cmp rdi,0
	je .epilogo
	cmp rdi,0
	je .epilogo

	;caso contrario iteramos sobre el directorio 
	mov rbx,rdi 
	mov r15, rsi 
	mov r14,[rbx + FANTASTRUCO_DIR_OFFSET]
	movzx r13, word [rbx + FANTASTRUCO_ENTRIES_OFFSET] 
	
	xor r12,r12 
	.ciclo: 
	cmp r12w,r13w
	jge .arquetipo

	;caso contrario iteramos sobre el directorio
	;tenemos que buscar el nombre de la habilidad 
	mov rax,[r14 + r12*8]; aqui tenemos el nombre de la habilidad
	lea rdi, [rax + DIRENTRY_NAME_OFFSET] 
	mov rsi, r15 ;aqui tenemos con el que queremos comparar
	call strcmp ; en rax tenemos el res 

	cmp eax,0
	je .usar_habilidad

	inc r12 
	jmp .ciclo

	.usar_habilidad: 
	mov rdi, rbx ;tenemos dir[i]
	mov r10,[r14 + r12*8]
	mov r10, [r10 + DIRENTRY_PTR_OFFSET]
	call r10
	jmp .epilogo

	.arquetipo:
	mov rdi,[rbx + FANTASTRUCO_ARCHETYPE_OFFSET]

	cmp rdi, 0 
	je .epilogo

	mov rsi,r15 
	call invocar_habilidad
	jmp .epilogo

	.epilogo: 
	add rsp,8
	pop rbx 
	pop r12
	pop r13
	pop r14 
	pop r15
	mov rsp,rbp
	pop rbp
	ret 
