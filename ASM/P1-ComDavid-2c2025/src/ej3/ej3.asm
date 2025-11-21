extern malloc
;########### SECCION DE DATOS
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

;usuario_t **asignarNivelesParaNuevosUsuarios(uint32_t *ids, uint32_t cantidadDeIds, uint8_t (*deQueNivelEs)(uint32_t)) {
;rdi -> arreglo de ids 
;rsi -> cantidad de ids 
; rdx -> funcion
global asignarNivelesParaNuevosUsuarios 
asignarNivelesParaNuevosUsuarios:
    push rbp 
    mov rbp,rsp 
    push rbx 
    push r12
    push r13
    push r14
    push r15
    sub rsp,8

    ;primero verificamos que la cant de id no sea nula 
    cmp rsi,0
    je .null_case
    ;si tenemos ids entonces preservamos todo en registros no volatiles

    mov rbx,rdi ;ids
    mov r12, rsi; cant ids 
    mov r13, rdx ;funcion 

    mov rdi,r12
    shl rdi,3
    call malloc
    ;en rax tenemos el array de punteros 
    mov r14,rax ;lo preservamos en r14 

    xor r8,r8 ;i = 0
    .for:
        cmp r8,r12
        jge .arr_case

        ;Ahora iteramos 

        ;primero calculamos el nivel del user
        mov rdi, [rbx + 4*r8] ;aqui tenemos la id
        push r8
        push rdi
        call r13
        pop rdi
        pop r8
        ;en rax tenemos el nivel de usuario 
        
        ;ahora tenemos que crear un nuevo puntero a usuario 
        mov r9, rdi ;preservamos la id 
        mov r10, rax 

        mov rdi,8
        push r8
        push r9
        push r10
        sub rsp,8 
        call malloc
        add rsp,8
        pop r10
        pop r9
        pop r8

        ;en rax tenemos el puntero al struct usuario 
        mov  dword [rax + USUARIO_ID_OFFSET],r9d
        mov  byte[rax + USUARIO_NIVEL_OFFSET],r10b

        ;ahora ponemos el puntero en el arr
        mov [r14 + r8*8],rax
        inc r8
        jmp .for

    .arr_case:
    mov rax,r14
    jmp .epilogo


    .null_case:
    xor rax,rax

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
