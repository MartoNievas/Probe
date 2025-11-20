extern malloc

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

;vamos primero con la funcion auxiliar 
;rdi -> user, rsi -> funcion 

contar_tuits_sobresalientes:
    push rbp 
    mov rbp,rsp
    push rbx 
    push r12
    push r13
    push r14
    push r15 
    sub rsp,8

    ;Preservamos rdi y rsi 
    mov rbx,rdi ;user
    mov r12, rsi ;funcion

    ;ahora buscamos la feed y luego la publicacion 

    mov r13, [rbx + USUARIO_FEED_OFFSET]
    mov r13, [r13 + FEED_FIRST_OFFSET] ;aqui tenemos la primera publicacion del feed 

    xor r14,r14;incializamos el contador 

    while:
        cmp r13,0
        je fin

        ;Caso contrario iteramos 
        ;primero verficamos que la id del autor del tuit coincide con la del usurariio
        mov rdi, [r13 + PUBLICACION_VALUE_OFFSET]
        mov r15d, dword [rbx + USUARIO_ID_OFFSET]
        cmp r15d, dword [rdi + TUIT_ID_AUTOR_OFFSET]
        jne siguiente ;si no son iguales pasamos al siguiente

        ;Si lo son vericamos si el tuit es sobresaliente
        call r12 

        ;Ahora verificamos en rax si es o no sobresaliente 
        cmp rax,0
        je siguiente
        ;Si no sumamos uno al contador 
        inc r14
        siguiente:
        mov r13, [r13 + PUBLICACION_NEXT_OFFSET]
        jmp while
        
    fin:
    mov rax,r14
    add rsp,8
    pop r15
    pop r14
    pop r13
    pop r12 
    pop rbx
    mov rsp,rbp
    pop rbp
    ret


; tuit_t **trendingTopic(usuario_t *usuario, uint8_t (*esTuitSobresaliente)(tuit_t *));
global trendingTopic 
trendingTopic:
    push rbp 
    mov rbp,rsp
    push rbx 
    push r12
    push r13
    push r14
    push r15 
    sub rsp,8

    ;verificamos que el user no sea null 

    cmp rdi,0 
    je .null

    ;Caso contrario preservamos rdi y rsi 
    mov rbx,rdi ;user 
    mov r12,rsi ;funcion 

    ;Ahora tenemos que conseguir la cantidad de tuits sobresalientes

    call contar_tuits_sobresalientes
    ;tenemos la cantidad en rax 

    ;verficamos si la cant es 0 
    cmp rax,0
    je .null

    ;Caso contrario tenemos que crear el arreglo 
    mov rdi,rax 
    mov r8,rax
    inc rdi 

    shl rdi,3 ;multiplicamos por 8 porque son punteros
    push r8
    sub rsp,8 
    call malloc ;asiganamos memoria 
    add rsp,8
    pop r8
    ;tenemos el puntero en rax, lo preservamos en r13 
    mov r13,rax 
    mov [r13 + r8*8], qword 0 ;Ponemos null en el ultimo indice

    xor r10,r10 ;i = 0 para ir poniendo los valores correpondientes 

    ;ahora tenemos que iterear sobre las publicaciones 

    mov r14, [rbx + USUARIO_FEED_OFFSET]
    mov r14, [r14 + FEED_FIRST_OFFSET] ;aqui tenemos la primera publicacion 

    .while: 
        cmp r14,0
        je .devolver_array


        ;Ahora verficamos si el tuit es sobresaliente y si corresponde al autor 

        mov rdi, [r14 + PUBLICACION_VALUE_OFFSET] ;tuit es temporal 
        mov r11d, dword [rdi + TUIT_ID_AUTOR_OFFSET] ;id del autor del tuit temporal
        cmp r11d, dword [rbx + USUARIO_ID_OFFSET] 
        jne .siguiente ;si no son iguales pasamos a la siguiente publicacion 

        ;si son iguales entonces tenemos que verficar si es tuit sobresaliente
        push rdi ;preservamos el tuit 
        push r10 ;preservamos el indice 
        call r12 ; en rax nos deja el resultado 
        pop r10 ;restauramos el indice
        pop rdi ;restauramos el tuit

        cmp rax,0
        je .siguiente ; si es 0 entonces vamos a la siguiente publicacion 

        ;caso contario guardamos el tuit y vamos a la siguiente publicacion 
        mov [r13 + r10*8],rdi 
        inc r10 

        .siguiente:
        mov r14, [r14 + PUBLICACION_NEXT_OFFSET] ;pub = pub->next
        jmp .while 

    .devolver_array:
    mov rax,r13
    jmp .epilogo

    .null:
    mov rax,0

    .epilogo:
    add rsp,8
    pop r15
    pop r14
    pop r13
    pop r12 
    pop rbx
    mov rsp,rbp
    pop rbp
    ret
