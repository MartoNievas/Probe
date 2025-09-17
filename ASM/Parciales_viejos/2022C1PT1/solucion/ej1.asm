global strArrayNew
global strArrayGetSize
global strArrayAddLast
global strArraySwap
global strArrayDelete

extern malloc
extern strClone
extern free

extern strdup ;vale usar esta???S


;########### SECCION DE DATOS
section .data

;########### SECCION DE TEXTO (PROGRAMA)
section .text

; str_array_t* strArrayNew(uint8_t capacity)
;(rdi de 8bits)dil->capacity
strArrayNew:
    ;prologo:
        push rbp
        mov rbp, rsp

        push r14 ;preservo param original y alineo pila
        push rdi
        

        mov rdi, 16 ;son los 16bytes que ocupa el struct, se los paso como param a malloc
        call malloc
        mov r14, rax
        ;ahora en rcx tengo el puntero a mi struct (strarray)

        
        pop rdi ;restauro capacity y alineo pila
        push r15

        mov byte [r14], 0 ;setteo size en 0
        mov byte [r14 + 1], dil ;setteo capacity con lo q me pasan por param

        ;el primer parametro ya esta pasado en rdi (8bits mas bajos)
        
        movzx rdi, dil ;extiendo de 8 a 64 para que calloc me lo tome bien
        imul rdi, 8 ; son los 8bytes que ocupa un char*, segundo param para el calloc
        call malloc
        ;en rax viene el array de strings de tamaño capacity

        mov [r14 + 8], rax ;setteo data al puntero q devolvio el calloc
        mov rax, r14 ;vuelvo a poner en rax el puntero al struct

    ;epilogo:
        pop r15
        pop r14
        pop rbp
        ret

; uint8_t  strArrayGetSize(str_array_t* a)
;rdi->a
strArrayGetSize:
    ;prologo:
        push rbp
        mov rbp, rsp

        movzx rax, byte [rdi] ;leo el size y extiendo (el size queda en los 8 bits mas bajos)

   ;epilogo:
        pop rbp
        ret


; void  strArrayAddLast(str_array_t* a, char* data)
;rdi->a
;rsi->data
strArrayAddLast:
    ;prologo:
        push rbp
        mov rbp, rsp
        push r14
        push r15
        push r13
        push r12
    
        movzx rcx, byte [rdi] ;rcx = (uint8_t)a->size
        movzx rdx, byte [rdi + 1] ; rdx = (uint8_t)a->capacity

        cmp rcx, rdx
        jae .epilogo ;a->size >=) a->capacity (Jump if Above or Equal)

        mov r14, rcx ;me guardo el a.size en r14 por si se rompe

        mov r13, [rdi + 8] ; r13 = a->data (puntero a array de strings)

        mov r15, rdi ;guardo una copia de a porq lo voy a pisar

        mov rdi, rsi ;le paso a strdup el string como param
        call strdup ;(hace strlen,malloc y strcpy internamente)
        ;En rax tengo el puntero al string copiado

        mov [r13 + r14*8], rax ;a->data[size] = data (que me pasaron por param)

        inc r14 ;size++
        mov byte [r15], cl ;a->size++ actualizado en el struct
    
    .epilogo:
        pop r12
        pop r13
        pop r15
        pop r14
        pop rbp
        ret


;void  strArraySwap(str_array_t* a, uint8_t i, uint8_t j)
;rdi- a
;rsi - i (sil)
;rdx - j (dl)
strArraySwap:
    ;prologo:
        push rbp
        mov rbp, rsp
        push r14
        push r15
        xor rcx, rcx

        movzx r15, byte[rdi] ;rcx = (uint8_t)a->size
        cmp sil, cl ;i >? a->size
        ja .epilogo
        cmp dl, cl ;j>?a->size
        ja .epilogo 

        mov r8, [rdi + 8] ;r8 = a->data (puntero a strings)

        movzx rcx, sil ; extiendo a 64 el i en rcx
        mov r9, [r8 + rcx*8 ] ;r9=a->data[i] (puntero al string i)

        movzx rdx, dl ;extiendo a 64 el j en rcx
        mov r10, [r8 + rdx*8] ;r10=a->data[j] (puntero al string j)
        
        mov [r8 + rcx*8], r10 ; data[i] = puntero a string j

        mov [r8 + rdx*8], r9    ; data[j] = puntero a string i

    .epilogo:
        pop r15
        pop r14
        pop rbp
        ret



; void  strArrayDelete(str_array_t* a)
;;rdi- a
strArrayDelete:
    ;prologo:
        push rbp
        mov rbp, rsp
        push r14
        push r15
        push r12
        push r13
        
    .preciclo:
        xor r15, r15;i=0
        movzx r14, byte[rdi] ;rdx = (uint8_t)a->size

        mov r13, rdi ;me guardo en r13 una copia de rdi
        mov r12, [r13 + 8] ;r12 = a->data

    .ciclo:
        cmp r15, r14 ;i ==? a->size
        je .fin

        mov rdi, [r12 + r15*8] ;rdi=a->data[i]
        call free ;parametro pasado
        inc r15 ;i++

        jmp .ciclo

    .fin:
        mov rdi, r12 ;free a->data
        call free

        mov rdi, r13 ;free rd a (el struct)
        call free

    .epilogo:
        pop r13
        pop r12
        pop r15
        pop r14
        pop rbp
        ret

