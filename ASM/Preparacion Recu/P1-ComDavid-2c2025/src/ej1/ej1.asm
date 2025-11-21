;########### SECCION DE DATOS
extern malloc
section .data

;########### SECCION DE TEXTO (PROGRAMA)
section .text

; Completar las definiciones (serán revisadas por ABI enforcer):
USUARIO_ID_OFFSET EQU 0
USUARIO_NIVEL_OFFSET EQU 4
USUARIO_SIZE EQU 8

PRODUCTO_USUARIO_OFFSET EQU 0
PRODUCTO_CATEGORIA_OFFSET EQU 8
PRODUCTO_NOMBRE_OFFSET EQU 17
PRODUCTO_ESTADO_OFFSET EQU 42
PRODUCTO_PRECIO_OFFSET EQU 44
PRODUCTO_ID_OFFSET EQU 48
PRODUCTO_SIZE EQU 56

PUBLICACION_NEXT_OFFSET EQU 0
PUBLICACION_VALUE_OFFSET EQU 8
PUBLICACION_SIZE EQU 16

CATALOGO_FIRST_OFFSET EQU 0
CATALOGO_SIZE EQU 8


;Vamos con la funcion auxiliar 

;rdi -> catalogo
contar_productos_que_cumplen_condiciones:
    push rbp 
    mov rbp,rsp
    push rbx 
    push r15 
    push r14 
    push r13
    push r12
    sub rsp,8
    

    xor rax,rax ; res = 0

    ;Ahora buscamos el primer producto 
    mov rbx,[rdi + CATALOGO_FIRST_OFFSET] ;curr

    ;Ahora itereamos 
    while:
        cmp rbx,0
        je fin ;si es nulo terminamos 
        ;si no es nulo iteramos 
        ;conseguimos user y prod 

        mov r12,[rbx + PUBLICACION_VALUE_OFFSET] ;prodcuto 
        mov r13, [r12 + PRODUCTO_USUARIO_OFFSET] ;usuario

        cmp  word [r12 + PRODUCTO_ESTADO_OFFSET], 1
        jne siguiente 
        cmp byte [r13 + USUARIO_NIVEL_OFFSET],  0
        je siguiente

        ;si cumple ambas condiciones entonces 
        inc rax



        siguiente:
        ;actualizamos el curr 
        mov rbx,[rbx + PUBLICACION_NEXT_OFFSET]
        jmp while
    fin:
    add rsp,8
    pop r12
    pop r13
    pop r14
    pop r15 
    pop rbx
    mov rsp,rbp 
    pop rbp 
    ret 


;producto_t* filtrarPublicacionesNuevasDeUsuariosVerificados (catalogo*)
;rdi -> catalogo
global filtrarPublicacionesNuevasDeUsuariosVerificados
filtrarPublicacionesNuevasDeUsuariosVerificados:
    push rbp 
    mov rbp,rsp
    push rbx 
    push r15 
    push r14 
    push r13
    push r12
    sub rsp,8
    

    ;primero preservamos rdi 
    mov rbx,rdi 
    call contar_productos_que_cumplen_condiciones ;contamos los proudcots que cumplen 

    ;en rax resultado 
    ;ahora reservamos espacio
    mov rdi,rax 
    inc rdi 
    shl rdi,3
    push rdi 
    sub rsp,8
    call malloc
    add rsp,8
    pop rdi 

    ;en rax tenemos el puntero 
    ;ahora ponemos el ultimo nulo 
    mov [rax + rdi - 8], qword 0


    ;ahora iteramos 
    mov r12,[rbx + CATALOGO_FIRST_OFFSET] ;curr
    xor rcx,rcx ; i = 0

    .while: 
        cmp r12,0
        je .fin

        ;conseguimos el producto y el usuario 
        mov r13,[r12 + PUBLICACION_VALUE_OFFSET] ;prod
        mov r14,[r13 + PRODUCTO_USUARIO_OFFSET] ;user

        ;ahora verificamos los campos 
        cmp word [r13 + PRODUCTO_ESTADO_OFFSET],  1
        jne .siguiente
        cmp byte [r14 + USUARIO_NIVEL_OFFSET], 0
        je .siguiente

        ;si se cumple ambas entonces 
        mov [rax + rcx*8], r13
        inc rcx

        .siguiente:
        mov r12,[r12 + PUBLICACION_NEXT_OFFSET]
        jmp .while
    .fin:
    add rsp,8
    pop r12
    pop r13
    pop r14
    pop r15 
    pop rbx
    mov rsp,rbp 
    pop rbp 
    ret 
