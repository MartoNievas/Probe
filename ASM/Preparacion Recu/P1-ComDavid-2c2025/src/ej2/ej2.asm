;########### SECCION DE DATOS
extern strncmp
extern free
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
;rdi -> publicacion 

removerAparicionesPosterioresDe: 
    push rbp 
    mov rbp,rsp 
    push rbx 
    push r12 
    push r13
    push r14
    push r15
    sub rsp,8


    ;Conseguimos prodcuto y usuario orginal 
    mov rbx, [rdi + PUBLICACION_VALUE_OFFSET] ;prodOriginal
    mov r12, [rbx + PRODUCTO_USUARIO_OFFSET] ;userOrginal

    mov r13, rdi ;prev = publicacion 
    mov r14, [rdi + PUBLICACION_NEXT_OFFSET] ;curr = publicacion-next

    while:
        cmp r14,0
        je fin

        ;si no iteramos 

        mov rcx, [r14 + PUBLICACION_VALUE_OFFSET] ;prodCurr
        
        
        ;conseguimos el nombre de la curr y la original 
        lea rdi,[rcx + PRODUCTO_NOMBRE_OFFSET]
        lea rsi, [rbx + PRODUCTO_NOMBRE_OFFSET]
        mov rdx, 25
        call strncmp ;Comparamos 

        ;el resultado lo tenemos en rax 
        cmp rax,0
        jne siguiente
        ;si son iguales verificamos la id del usurario 
        mov rcx, [r14 + PUBLICACION_VALUE_OFFSET]
        mov r15, [rcx + PRODUCTO_USUARIO_OFFSET] ;userCurr
        movzx r15, dword[r15 + USUARIO_ID_OFFSET] ;id_curr
        cmp r15d, dword[r12 + USUARIO_ID_OFFSET]
        jne siguiente
        ;si son iguales 

        ;primero preservamos la actual
        mov rdi, [r14 + PUBLICACION_VALUE_OFFSET]
        call free

        mov rdi, r14 
        

        mov r14, [r14 + PUBLICACION_NEXT_OFFSET] ; curr = curr->next

        mov [r13 + PUBLICACION_NEXT_OFFSET], r14 ;prev->next = curr
        
        call free

        jmp while

        siguiente:
        mov r13,r14
        mov r14, [r14 + PUBLICACION_NEXT_OFFSET]
        jmp while


    fin:
    add rsp,8
    pop r15
    pop r14
    pop r13 
    pop r12
    pop rbx 
    mov rsp,rbp
    pop rbp
    ret


;catalogo* removerCopias(catalogo* h)
global removerCopias
removerCopias:
     push rbp 
    mov rbp,rsp 
    push rbx 
    push r12 
    push r13
    push r14
    push r15
    sub rsp,8

    ;buscamos la primera publicacion 
    mov r12,rdi
    mov rbx, [rdi + CATALOGO_FIRST_OFFSET]

    .while:
        cmp rbx,0 
        je .fin

        mov rdi,rbx
        call removerAparicionesPosterioresDe

        mov rbx,[rbx + PUBLICACION_NEXT_OFFSET]
    jmp .while
    .fin:
    mov rax,r12
    add rsp,8
    pop r15
    pop r14
    pop r13 
    pop r12
    pop rbx 
    mov rsp,rbp
    pop rbp
    ret
