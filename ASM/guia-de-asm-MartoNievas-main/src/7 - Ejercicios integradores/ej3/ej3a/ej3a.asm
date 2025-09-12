extern malloc
extern free
;########### SECCION DE DATOS
section .data

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


;int contar_casos_por_nivel(caso_t* arreglo_casos[rdi], int largo [rsi], int nivel[rdx])
contar_casos_por_nivel: 
    push rbp ;pila alineda 
    mov rbp,rsp 
    push rbx
    push r12
    push r13
    push r14

    ;inicializamos res en -1 
    mov rax,-1
    cmp rdi,0 ;si el puntero al arreglo es nulo, saltamos al epilogo y retornamos -1 
    je epilogo

    ;si es 0 no entro en el ciclo
    xor rax,rax ;res = 0;
    
    mov rbx,rdi
    xor r12,r12 ;int i = 0;
    ;caso contrario 
    ciclo: 

    cmp r12,rsi
    jge epilogo

    mov r14,r12
    imul r14,CASO_SIZE
    mov r13,[rbx + r14 + USUARIO_ID_OFFSET]
    mov r13,[r13 + USUARIO_NIVEL_OFFSET] 

    cmp r13,rdx 
    jne no_iguales ;si no son iguales no aumento el contador

    ;caso contrario si lo hago 
    inc rax ;res++
    inc r12 ;i ++ 
    jmp ciclo

    no_iguales:
    inc r12 
    jmp ciclo
    

epilogo:
pop r14
pop r13
pop r12
pop rbx
mov rsp,rbp 
pop rbp
ret

;segmentacion_t* segmentar_casos(caso_t* arreglo_casos, int largo)
global segmentar_casos
segmentar_casos:
push rbp
mov rbp,rsp
push rbx
push r15 
push r14 
push r13 
push r12 
sub rsp,8
;guardamos la longitud en la pila porque la vamos a nesecitar 
;stack frame y stack alineado 

;verifiquemos que el puntero no es nulo 
xor rax,rax
cmp rdi, 0
je .epilogo 

mov rbx,rdi ;guardamos el puntero al arreglo de casos en rbx 

.crear_segmento: 

mov rdi,SEGMENTACION_SIZE
call malloc 

;verficamos que no sea nulo el puntero 
cmp rax,0
je epilogo

mov r15,rax 

.conteo_de_casos_creacion_de_arreglos: 
.caso_nivel0:
mov rdx,0 
call contar_casos_por_nivel
cmp rax,0
je .nivel0_es_cero
imul rax,CASO_SIZE
mov rdi,rax 
call malloc
mov r12,rax ;aca guardamos el puntero al arreglo casos_nivel0 

;vamos con el segundo arreglo 
.casos_nivel1:
inc rdx 
mov rdi,rbx ;buscamos en el registro la base del arreglo 
call contar_casos_por_nivel
cmp rax,0
je .nivel1_es_cero
imul rax,CASO_SIZE
mov rdi,rax 
call malloc ;aca guardamos casos_nivel1
mov r13,rax
;camos con el tercer arreglo
.casos_nivel2:
inc rdx
mov rdi,rbx;buscamos el puntero al arreglo en rbx
call contar_casos_por_nivel
cmp rax,0
je .nivel2_es_cero
imul rax,CASO_SIZE
mov rdi,rax 
call malloc
mov r14,rax ;guardamos casos_nivel2

jmp .pre_ciclo
;r12 = nivel0, r13 = nivel1, r14 = nivel2 

.nivel0_es_cero: 
xor r12,r12 
jmp .casos_nivel1

.nivel1_es_cero: 
xor r13,r13
jmp .casos_nivel2

.nivel2_es_cero:
xor r14,r14 
;de aqui continumamos porque el resto ya esta seteado en null o con un puntero 


;entonces tenemos
;rbx = base del arreglo 
;r12 = casos_nivel0
;r13 = casos_nivel1
;r14 = casos_nivel2
;r15 = res puntero a segmento 

;ahora tenemos que ir copiando los casos 
.pre_ciclo:
xor r10,r10 ;i = 0
xor rdi,rdi ;i0 = 0
xor rdx,rdx ;i1 = 0
xor rcx,rcx ;i2 = 0

;ahora ponemos los casos en donde corresponeden 

.ciclo_asignacion_de_casos: 
cmp r10,rsi
jge .fin_ciclo

mov r9,r10
imul r9,CASO_SIZE

mov r11,[rbx + r9] ;en r11 = arreglo_casos[i]
mov r8, [r11  + CASO_USUARIO_OFFSET] ;r8 = arreglo_casos[i].usuario
mov r8d, dword [r8 + USUARIO_NIVEL_OFFSET] ;r8 = arreglo_casos[i].ususario->nivel

cmp r8d,0
je .es_nivel0

cmp r8d,1
je .es_nivel1

;si no es nivel 0, ni 1, entonces es nivel 2 
;lo colocamos en la posicion i2
.es_nivel2:
    mov r9,rcx 
    imul r9,CASO_SIZE
    mov [r14 + r9],r11 ;res->casos_nivel_3[i2] = arreglo_casos[i] 
    inc rcx ;i2++
    inc r10 ;i++
    jmp .ciclo_asignacion_de_casos

.es_nivel0:
    mov r9,rdi
    imul r9,CASO_SIZE
    mov [r12 + r9],r11
    inc rdi ;i0++
    inc r10 ;i++
    jmp .ciclo_asignacion_de_casos

.es_nivel1: 
    mov r9,rdx
    imul r9,CASO_SIZE
    mov [r13 + r9],r11
    inc rdx ;i1++
    inc r10 ;i++
    jmp .ciclo_asignacion_de_casos




.fin_ciclo: 
;ahora debemos pasar poner los arreglos de casos en segmentos 
mov [r15 + SEGMENTACION_CASOS0_OFFSET], r12 
mov [r15 + SEGMENTACION_CASOS1_OFFSET], r13
mov [r15 + SEGMENTACION_CASOS2_OFFSET], r14


mov rax,r15

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