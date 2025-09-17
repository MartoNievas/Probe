global templosClasicos
global cuantosTemplosClasicos

extern malloc

;########### SECCION DE TEXTO (PROGRAMA)
section .text

;templo* templosClasicos_c(templo *temploArr, size_t temploArr_len)
;rdi->*temploArr
;rsi->temploArr_len
templosClasicos:
    .prologo:
        push rbp
        mov rbp, rsp
        
        push r14
        push r15

    .preciclo:
        push rdi
        push rsi ;preservo los parametros originales antes del call (pila alineada)

        call cuantosTemplosClasicos ;los parametros ya estan pasados ya q son los mismos
        ;en rax tengo el resultado

        mov rdi, rax ;rdi tiene la ctad de templos clasicos
        imul rdi, 24 ;multiplico los clasicos x 24bytes (calculando la cantidad de espacio para pedirle al malloc)

        call malloc ;el parametro ya esta en rdi (24*clasicos)
        ;en rax tengo el resultado

        pop rsi
        pop rdi ;restauro los params originales

        xor r9, r9; i=0
        xor r10, r10; j=0

    .ciclo:
        cmp r9, rsi ; i ==? temploArr_len
        je .epilogo 

        xor rcx, rcx
        xor rdx, rdx

        mov cl, byte [rdi] ; cl=temploarr[i].clarga
        mov dl, byte [rdi + 16] ;dl=temploarr[i].ccorta


    .cuenta:
    
        shl rdx, 1 ;dl=temploarr[i].ccorta * 2
        add rdx, 1 ;dl=temploarr[i].ccorta + 1

        cmp rcx, rdx ;cl == ? dl
        jne .next_templo

        mov rcx, [rdi] ;rcx=temploArr[i]

        mov r14, r10
        imul r14, 24 ;multiplico por los bytes de la estructura

        mov [rax + r14], rcx  ;templos_clasicos_arr[j] = temploArr[i]; (primeros 8bytes)

        mov rcx, [rdi + 8]
        mov [rax + r14  + 8], rcx ;(segundos 8bytes)

        mov rcx, [rdi + 16]
        mov [rax + r14 + 16], rcx ; (terceros 8bytes)

        inc r10 ;j++

    .next_templo:

        inc r9 ;i++
        add rdi, 24 ;paso al sig templo
        jmp .ciclo


     .epilogo:
        pop r15
        pop r14
        pop rbp
        ret


   

;uint32_t cuantosTemplosClasicos_c(templo *temploArr, size_t=64bits temploArr_len){
;rdi->*temploArr
;rsi->temploArr_len
cuantosTemplosClasicos:
    .prologo:
    push rbp 
    mov rbp,rsp 
    

    xor rax,rax 
    cmp rdi,0 
    je .epilogo

    xor r9,r9 ; int i = 0

    .ciclo: 
    cmp r9,rsi 
    je .epilogo

    xor rdx,rdx 
    xor rcx,rcx 

    mov cl, [rdi]
    mov dl, [rdi + 16] 

    inc r9 
    add rdi,24 ;siguiente templo 


    .cuentas: 

    shl rdx,1
    inc rdx 

    cmp rdx,rcx 
    jne .ciclo 

    inc rax     
    jmp .ciclo


    .epilogo:
    mov rsp,rbp 
    pop rbp 
        ret