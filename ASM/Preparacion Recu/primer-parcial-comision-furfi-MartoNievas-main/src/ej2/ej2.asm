extern free

;########### SECCION DE DATOS
section .data

;########### SECCION DE TEXTO (PROGRAMA)
section .text

; Completar las definiciones (serán revisadas por ABI enforcer):
TUIT_MENSAJE_OFFSET EQU 0
TUIT_FAVORITOS_OFFSET EQU 140
TUIT_RETUITS_OFFSET EQU 142
TUIT_ID_AUTOR_OFFSET EQU 144
TUIT_SIZE EQU 148

PUBLICACION_NEXT_OFFSET EQU 0
PUBLICACION_VALUE_OFFSET EQU 8
PUBLICACION_SIZE EQU 16

FEED_FIRST_OFFSET EQU 0 
FEED_SIZE EQU 8


USUARIO_FEED_OFFSET EQU 0;
USUARIO_SEGUIDORES_OFFSET EQU 8; 
USUARIO_CANT_SEGUIDORES_OFFSET EQU 16; 
USUARIO_SEGUIDOS_OFFSET EQU 24; 
USUARIO_CANT_SEGUIDOS_OFFSET EQU 32; 
USUARIO_BLOQUEADOS_OFFSET EQU 40; 
USUARIO_CANT_BLOQUEADOS_OFFSET EQU 48; 
USUARIO_ID_OFFSET EQU 52; 
USUARIO_SIZE EQU 56

;Vamos con la funcion auxiliar 

;rdi -> feed, rsi -> usuarioABorrar
borrar_publicaciones: 
    push rbp 
    mov rbp,rsp
    push rbx 
    push r12
    push r13
    push r14
    push r15 
    sub rsp,8


    ;buscamos el primero del feed 
    xor rbx,rbx ;prev = null
    mov r12, [rdi + FEED_FIRST_OFFSET] ;curr = feed->first 

    ;Ahora vamos a iterar 
    
    while:
        cmp r12,0
        je fin

        ;verificamos si el id del autor coincide 
        mov r13, [r12 + PUBLICACION_VALUE_OFFSET]
        mov r13, [r13 + TUIT_ID_AUTOR_OFFSET]
        cmp r13d, dword [rsi + USUARIO_ID_OFFSET]
        jne siguiente 

        ;Si son iguales entonces verificamos 
        mov r14, r12 ;to_free = curr 
        mov r12, [r12 + PUBLICACION_NEXT_OFFSET] ; curr = curr->next

        ;verificamos si prev es null 
        cmp rbx,0
        jne caso_medio 
        ; si son iguales entonces estamos borrando la cabeza 
        mov [rdi + FEED_FIRST_OFFSET], r12 ;feed->first = curr;
        jmp liberar

        caso_medio:
        mov [rbx + PUBLICACION_NEXT_OFFSET], r12

        liberar:
        ;liberamos to_free 
        push rdi ;preservamos feed 
        push rsi ;preservamos usuario
        mov rdi, r14 
        call free 
        pop rsi ;restauramos usuario
        pop rdi ;restauramos feed 

        jmp while 


        siguiente:
        mov rbx, r12
        mov r12,[r12 + PUBLICACION_NEXT_OFFSET]
        jmp while

    fin:
    add rsp,8
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx 
    mov rbp,rsp
    pop rbp
    ret


; void bloquearUsuario(usuario_t *usuario, usuario_t *usuarioABloquear);
;rdi -> userBloqueador, rsi -> usuarioABloquear
global bloquearUsuario 
bloquearUsuario:
    push rbp 
    mov rbp,rsp
    push rbx 
    push r12
    push r13
    push r14
    push r15 
    sub rsp,8

    ;Buscamos la lista de usuarios bloqueados por usuario bloquedaor 
    mov rbx,[rdi + USUARIO_BLOQUEADOS_OFFSET]
    movzx r8, dword [rdi + USUARIO_CANT_BLOQUEADOS_OFFSET]

    mov [rbx + r8*8], rsi ;bloqueados[cantBloqueados] = usuarioABloquear
    inc [rdi + USUARIO_CANT_BLOQUEADOS_OFFSET]

    ;ahora llamamos a la funcion auxiliar 

    mov rbx,rdi ;Bloqueador
    mov r12, rsi ;Bloqueado

    mov rdi,[rbx + USUARIO_FEED_OFFSET]
    mov rsi, r12 

    call borrar_publicaciones

    ;ahora al revez 

    mov rdi, [r12 + USUARIO_FEED_OFFSET]
    mov rsi, rbx 
    call borrar_publicaciones


    add rsp,8
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx 
    mov rbp,rsp
    pop rbp
    ret
