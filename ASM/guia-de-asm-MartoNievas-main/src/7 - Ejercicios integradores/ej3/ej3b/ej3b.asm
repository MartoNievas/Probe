;########### SECCION DE DATOS
extern strncmp
section .data
cadena_clt db "CLT", 0    ; Con null terminator
cadena_rbo db "RBO", 0    ; Con null terminator
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

global resolver_automaticamente

;void resolver_automaticamente(funcionCierraCasos* funcion[rdi], caso_t* arreglo_casos[rsi], caso_t* casos_a_revisar[rdx], int largo)
resolver_automaticamente:
    push rbp 
    mov rbp,rsp
    push rbx 
    push r15
    push r14
    push r13
    push r12
    sub rsp,8 ;stack frame alineado 
    mov [rbp- 48],rcx ;guardamos el largo en la pila por si lo nesecitamos 

    ;verficamos que los punteros no sean nulos 
    .comprobacion_de_punteros:
    cmp rdi,0
    je .epilogo
    cmp rsi,0
    je .epilogo
    cmp rdx,0 
    je .epilogo

    .preservacion_de_datos:
    ;una vez verficado los punteros los pasamos a registros no volatiles 
    mov rbx,rdi ; en rbx esta la funcion 
    mov r15, rsi ; en r15 esta el arreglo de casos 
    mov r14,rdx ;en r14 esta el puntero al arreglo de casos por revisar 

    xor r10,r10 ;int i = 0
    xor r11,r11 ;int i1 = 0
    .ciclo_de_revision: 
    cmp r10,rcx ;comparamos para ver si el ciclo termino 
    je .epilogo
    ;caso contrario vamos con la revision de los casos 

    ;calculamos el offset 
    mov r9,r10
    imul r9,CASO_SIZE
    mov rdi,[r15 + r9] ;en rdi = arreglos_casos[i]
    mov rsi,[r15 + r9 + CASO_USUARIO_OFFSET]
    mov rsi, [rsi + USUARIO_NIVEL_OFFSET]

    .verficacion_de_nivel: 

    cmp rsi,1
    je .cambio_de_estado
    cmp rsi,2
    je .cambio_de_estado

    ;caso contrario es nivel 0 o no cumple con las condiciones anteriores lo metemos en casos_por_revisar 
    .no_cambia_el_estado:
    mov rdx,r11
    imul rdx, CASO_SIZE
    mov [r14 + rdx],rdi ;casos_por_revisar[i1] = arreglo_casos[i]
    inc r10 
    inc r11
    jmp .ciclo_de_revision

    .cambio_de_estado: 
    
    push rdi
    push r10
    push r11
    push r9
    lea rdi,[r15 + r9]
    call rbx ;llamamos a la funcion de comparacion 
    pop r9
    pop r11
    pop r10 
    pop rdi ;restauramos el valor del caso 
    ;ahora tenemos el valor en rax , verificamos si es 0 o 1 

    cmp rax,0
    je .comparar_categoria
    ;caso contrario lo marcamos como caso cerrado favorablemente 

    mov [r15 +r9 + CASO_ESTADO_OFFSET], word 1;le ponemos 1 como estado 
    inc r10
    jmp .ciclo_de_revision

    .comparar_categoria: 
    ;debemos comparar si pertenece a la categoria RBO o CLT
    push rdi
    push r10
    push r11 
    push r9

    lea rdi,[r15 + r9 + CASO_CATEGORIA_OFFSET]
    lea rsi, cadena_rbo
    mov rdx,4
    call strncmp
    mov rcx,[rbp-48]
    pop r9
    pop r11 
    pop r10 
    pop rdi
    cmp rax,0
    je .cambiar_de_estado

    

    push rdi
    push r10
    push r11
    push r9

    lea rdi, [r15 + r9 + CASO_CATEGORIA_OFFSET]
    lea rsi,cadena_clt
    mov rdx,4
    call strncmp
    mov rcx,[rbp - 48]
    pop r9
    pop r11
    pop r10
    pop rdi 
    cmp rax,0
    jne .no_cambia_el_estado

    .cambiar_de_estado:

    ;ahora cambiamos a estado 2
    mov [r15 + r9 + CASO_ESTADO_OFFSET], word 2
    
    .next: 
        inc r10
        jmp .ciclo_de_revision



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
