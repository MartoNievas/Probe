%define SYS_WRITE 1             ;Direcctivas del procesador 
%define SYS_EXIT 60     
%define STDOUT 1        

section .data                   ;Seccion de datos
msg: db '¡Hola Mundo!', 10      ;Declaramos un string 
len equ $ - msg                 ;con esto sacamos su longitud 

global _start                   ;funcion de acceso globlal
section .text                   ;segmento de codigo
_start:                         ;funcion start             
    mov rax, SYS_WRITE                                          
    mov rdi, STDOUT                                             
    mov rsi, msg                                                
    mov rdx, len                                                
    syscall                                                     
                                                                
    mov rax, SYS_EXIT                                            
    mov rdi, 0                                                  
    syscall                                                     
