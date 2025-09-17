global templosClasicos
global cuantosTemplosClasicos

extern malloc

;########### SECCION DE TEXTO (PROGRAMA)
section .text

;templo* templosClasicos_c(templo *temploArr, size_t temploArr_len)
;rdi->*temploArr
;rsi->temploArr_len
templosClasicos:
        push rbp 
        mov rbp,rsp 
        push rbx 
        push r15
        push r14
        push r13
        
        ;verficamos si el puntero es nulo 
        xor rax,rax 
        cmp rdi, 0
        je .epilogo

        ;caso creamos el puntero al nuevo arreglo de templos clasicos

        push rdi 
        push rsi 
        call cuantosTemplosClasicos

        imul rax,24 
        mov rdi,rax 

        call malloc ;en malloc tenemos el puntero al arreglo 

        pop rsi
        pop rdi 

        mov rbx,rdi 
        mov r15,rsi 

        xor r8,r8 ; int i = 0
        xor r9,r9 ; int i1 = 0 

        .ciclo: 
        cmp r8,r15
        jge .epilogo

        ;caso contrario vamos iterando sobre el array 
        mov r10,r8 
        imul r10,24 
        movzx r11, byte [rbx + r10 + 0] ;aca tenemos el lado largo 
        movzx r13, byte [rbx + r10 + 16] ;aca tenemos el lado corto 

        .cuentas: 

        shl r13,1
        inc r13 
        cmp r13,r11
        jne .next 

        ;si son iguales debemos copiar el struct 
        ;calculamos el offset 
        mov r14,r9
        imul r14,24 
        
        ;ahora debemos ir trayendo el struct de a 8 bytes
        mov rcx,[rbx + r10 + 0]
        mov [rax + r14 + 0],rcx ;primero 8 bytes 

        mov rcx,[rbx + r10 + 8]
        mov [rax + r14 + 8],rcx ;segundos 8 bytes 

        mov rcx, [rbx + r10 + 16]
        mov [rax + r14 + 16],rcx

        inc r9 ;i1++;

        .next: 
        inc r8 
        jmp .ciclo
 

        .epilogo: 
        pop r13
        pop r14
        pop r15
        pop rbx 
        mov rsp,rbp 
        pop rbp 
        ret


   

;uint32_t cuantosTemplosClasicos_c(templo *temploArr, size_t=64bits temploArr_len){
;rdi->*temploArr
;rsi->temploArr_len
cuantosTemplosClasicos:
        push rbp 
        mov rbp,rsp 
        push rbx 
        push r14        

        ;verificamos si el puntero es nulo 
        xor rax,rax ;uint32_t res = 0 
        cmp rdi, 0 
        je .epilogo 

        xor r8,r8 ; int i = 0; 

        .ciclo: 
        cmp r8,rsi 
        jge .epilogo

        mov r9,r8 
        imul r9,24 
        movzx rdx, byte [rdi + r9 + 0] ;tenemos el lado largo 
        movzx rcx, byte [rdi + r9 + 16]; tenemos el lado corto 

        .cuenta: 
        shl rcx,1
        inc rcx 
        cmp rcx,rdx 
        jne .next 

        inc rax 

        .next: 
        inc r8 
        jmp .ciclo

        .epilogo: 
        pop r14
        pop rbx 
        mov rsp,rbp 
        pop rbp 
        ret