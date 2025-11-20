extern malloc
extern strncpy
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

;funcion auxiliar 
;por rdi -> tuit, por rsi -> feed
nueva_publicacion: 
    push rbp 
    mov rbp,rsp

    push rbx 
    push r12
    ;verificamos que ningun puntero sea nulo 
    cmp rdi,0
    je fin
    cmp rsi,0
    je fin

    ;Preservamos en registros no volatiles, los parametros de entrada
    mov rbx,rdi ;tuit
    mov r12, rsi ;feed
    ;ahora debemos crear la publicacion
    
    mov rdi,PUBLICACION_SIZE
    call malloc

    ;en eax tenemos el puntero
    ;verificamos que no sea nulo
    cmp eax,0
    je fin

    ;Ahora asignamos los valores 

    mov [eax + PUBLICACION_VALUE_OFFSET], rbx
    mov r8,[r12 + FEED_FIRST_OFFSET] ;guardamos el puntero al primero del feed
    mov [eax + PUBLICACION_NEXT_OFFSET], r8
    ;por ultimo ponemos la nueva publicacion como primero 
    mov [r12 + FEED_FIRST_OFFSET],eax

    fin:
    pop r12 
    pop rbx
    mov rsp,rbp
    pop rbp
    ret


; tuit_t *publicar(char *mensaje, usuario_t *usuario);
; me pasan por rdi -> mensaje, por rsi -> user 
global publicar
publicar:
    push rbp 
    mov rbp,rsp

    push rbx 
    push r12
    push r13
    push r14
    push r15 
    sub rsp,8

    ;verificamos que los punteros no sean nulos 
    cmp rdi,0
    je .fin
    cmp rsi,0
    je .fin 

    ;si ninguno es nulo entonces los preservamos en registros 
    ;no volatiles 

    mov rbx,rdi ;mensaje 
    mov r12,rsi ;user 

    ;ahora creamos un tuit 

    mov rdi,TUIT_SIZE
    call malloc 
    ;en rax tenemos el puntero al tuit 
    mov r13, rax
    ;ahora asginemos cada campo 
    mov r8d, dword [r12 + USUARIO_ID_OFFSET] ;id del usuario
    mov [r13 + TUIT_ID_AUTOR_OFFSET], r8d

    mov [r13 + TUIT_RETUITS_OFFSET], word 0
    mov [r13 + TUIT_FAVORITOS_OFFSET], word 0

    lea rdi,[r13 + TUIT_MENSAJE_OFFSET] ;direccion base del arreglo de char
    mov rsi,rbx ;puntero al mensaje a clonar
    mov rdx,140 ;cantidad maxima de chars 
    call strncpy 

    ;Una vez que tenemos el tuit ya armado, tenemos que copiarlo al feed 
    mov rdi,r13
    mov rsi, [r12 + USUARIO_FEED_OFFSET]     
    call nueva_publicacion

    ;Una vez que tenemos el nuevo feed del usuario ahora tenemos que hacerlo con los seguidores 


    mov r14d, dword [r12 + USUARIO_CANT_SEGUIDORES_OFFSET] ;aqui tenemos la canitdad de seguidores 
    mov r15, [r12 + USUARIO_SEGUIDORES_OFFSET] ;aqui tenemos los seguidores 

    ;Ahora un bucle 
    xor r11,r11
    .for: 
        cmp r11d,r14d
        jge .epilogo

        ;sino iteramos 
        mov rdi,r13
        mov rdx, [r15 + r11*8] ;aqui tengo un puntero a un seguidor puntual
        mov rsi, [rdx + USUARIO_FEED_OFFSET]

        push r10
        push r11 
        call nueva_publicacion
        pop r11 
        pop r10

        inc r11
        jmp .for


    .epilogo: 
    mov rax,r13
    .fin:
    add rsp,8
    pop r15
    pop r14
    pop r13
    pop r12 
    pop rbx
    mov rsp,rbp
    pop rbp
    ret
ret
