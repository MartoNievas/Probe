extern strncmp
extern malloc
;########### SECCION DE DATOS
section .data
cadena_clt db "CLT", 0    ; Con null terminator
cadena_rbo db "RBO", 0    ; Con null terminator
cadena_ksc db "KSC",0
cadena_kdt db "KDT",0
;########### SECCION DE TEXTO (PROGRAMA)
section .text
; Completar las definiciones (serán revisadas por ABI enforcer):
USUARIO_ID_OFFSET EQU 0
USUARIO_NIVEL_OFFSET EQU 4
USUARIO_SIZE EQU 8

CASO_CATEGORIA_OFFSET EQU 0
CASO_ESTADO_OFFSET EQU 4
CASO_USUARIO_OFFSET EQU 8
CASO_SIZE EQU 16

SEGMENTACION_CASOS0_OFFSET EQU 0
SEGMENTACION_CASOS1_OFFSET EQU 8
SEGMENTACION_CASOS2_OFFSET EQU 16
SEGMENTACION_SIZE EQU 24

ESTADISTICAS_CLT_OFFSET EQU 0
ESTADISTICAS_RBO_OFFSET EQU 1
ESTADISTICAS_KSC_OFFSET EQU 2
ESTADISTICAS_KDT_OFFSET EQU 3
ESTADISTICAS_ESTADO0_OFFSET EQU 4
ESTADISTICAS_ESTADO1_OFFSET EQU 5
ESTADISTICAS_ESTADO2_OFFSET EQU 6
ESTADISTICAS_SIZE EQU 7

;void actualizar_estadistica(caso_t *caso[rdi], estadisticas_t *res[rsi]) {
actualizar_estadistica:
push rbp 
mov rbp,rsp
push rbx
push r15 
push r14
push r13
push r12
sub rsp,8 ;stack alineado 

;verfiquemos que ningun puntero es nulo 
cmp rdi,0
je epilogo
cmp rsi,0
je epilogo
;si ninguno es nulo vamos con las verificaciones 
;primero verficamos la categoria
mov r15,rdi ;movemos a r15el puntero al caso
mov r14,rsi ;movemos a r14 el puntero a las estadisticas

lea rdi,[r15 + CASO_CATEGORIA_OFFSET]
lea rsi,cadena_clt
mov rdx,4
call strncmp ;comparamos y vemos que nos devuelve 
jnz probar_rbo

inc byte [r14 + ESTADISTICAS_CLT_OFFSET] ;incrementamos clt 
jmp contar_estado

probar_rbo:
lea rdi, [r15 + CASO_CATEGORIA_OFFSET]
lea rsi,cadena_rbo
mov rdx,4 
call strncmp
jnz probar_ksc

inc byte [r14 + ESTADISTICAS_RBO_OFFSET] ;incrementamos rbo
jmp contar_estado

probar_ksc: 
lea rdi, [r15 + CASO_CATEGORIA_OFFSET]
lea rsi,cadena_ksc
mov rdx,4
call strncmp
jnz probar_kdt

inc byte [r14 + ESTADISTICAS_KSC_OFFSET]

probar_kdt: 
lea rdi, [r15 + CASO_CATEGORIA_OFFSET]
lea rsi,cadena_kdt
mov rdx,4
call strncmp
jnz contar_estado

inc byte [r14 + ESTADISTICAS_KDT_OFFSET]

contar_estado: 

;ahora veamos a que estado pertenece 
mov edi, dword [r15 + CASO_ESTADO_OFFSET]

cmp edi,0
jne probar_estado1

inc byte [r14 + ESTADISTICAS_ESTADO0_OFFSET]
jmp epilogo

probar_estado1: 
cmp edi,1
jne probar_estado2

inc  byte [r14 + ESTADISTICAS_ESTADO1_OFFSET]
jmp epilogo

probar_estado2: 
inc byte [r14 + ESTADISTICAS_ESTADO2_OFFSET]


epilogo:
add rsp,8
pop r12
pop r13
pop r14
pop r15
pop rbx
mov rsp,rbp
pop rbp
ret


global calcular_estadisticas

;void calcular_estadisticas(caso_t* arreglo_casos[rdi], int largo[rsi], uint32_t usuario_id[rdx])
calcular_estadisticas:
    push rbp
    mov rbp,rsp
    push rbx
    push r15
    push r14
    push r13
    push r12
    sub rsp,8 ;stack frame alineado 

    ;verficamos que los punteros no sean nulos 
    xor rax,rax ;res = NULL;
    cmp rdi,0
    je .epilogo

    ;movemos parametros de entrada a registros no volatiles 
    mov rbx,rdi
    mov r15,rsi
    mov r14,rdx

    ;creamos el la estadistica con malloc 
    mov rdi,ESTADISTICAS_SIZE
    call malloc
    ;verficquemos que el puntero no sea nulo 
    cmp rax,0
    je .epilogo

    ;caso contrario lo moevemos a un registro no volatil e  inicializamos todos sus campos en 0 
    mov r13,rax 
    mov [r13 + ESTADISTICAS_CLT_OFFSET], byte 0
    mov [r13 + ESTADISTICAS_RBO_OFFSET], byte 0
    mov [r13 + ESTADISTICAS_KSC_OFFSET], byte 0
    mov [r13 + ESTADISTICAS_KDT_OFFSET], byte 0
    mov [r13 + ESTADISTICAS_ESTADO0_OFFSET], byte 0
    mov [r13 + ESTADISTICAS_ESTADO1_OFFSET], byte 0
    mov [r13 + ESTADISTICAS_ESTADO2_OFFSET], byte 0

    ;ahora veamos si usuario != 0 o == 0
    xor rcx,rcx ;int i = 0
    cmp r14,0
    je .contar_todos


    .contar_usuario:
    cmp rcx,r15
    jge .fin_ciclos

    mov r9,rcx 
    imul r9,CASO_SIZE
    mov r8,[rbx + r9 + CASO_USUARIO_OFFSET] ;
    mov r8, [r8 + USUARIO_ID_OFFSET]
    cmp r8, r14 ;comparamos si las ids son las mismas 
    jne .next

    lea rdi,[rbx + r9] ;pasamos la direccion del i-esimo elemento del arreglo 
    mov rsi,r13 ;pasamos el puntero a la estructura estadistica_t
    push r9
    push rcx
    call actualizar_estadistica
    pop rcx
    pop r9

    .next:
    inc rcx
    jmp .contar_usuario

    .contar_todos: 
    cmp rcx,r15
    jge .fin_ciclos

    mov r9,rcx 
    imul r9,CASO_SIZE
    lea rdi,[rbx + r9]
    mov rsi,r13
    push r9
    push rcx
    call actualizar_estadistica
    pop rcx
    pop r9

    inc rcx
    jmp .contar_todos

    .fin_ciclos:   
        mov rax, r13


    .epilogo:
    add rsp,8 
    pop r12
    pop r13
    pop r14
    pop r15
    pop rbx
    mov rsp,rbp
    pop rbp
    ret
