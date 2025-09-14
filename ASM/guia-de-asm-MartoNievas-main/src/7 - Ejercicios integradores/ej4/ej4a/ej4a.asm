extern malloc
extern sleep
extern wakeup
extern create_dir_entry

section .rodata
; Acá se pueden poner todas las máscaras y datos que necesiten para el ejercicio
sleep_name: DB "sleep", 0
wakeup_name: DB "wakeup", 0

section .text
; Marca un ejercicio como aún no completado (esto hace que no corran sus tests)
FALSE EQU 0
; Marca un ejercicio como hecho
TRUE  EQU 1

; Marca el ejercicio 1A como hecho (`true`) o pendiente (`false`).
;
; Funciones a implementar:
;   - init_fantastruco_dir
global EJERCICIO_1A_HECHO
EJERCICIO_1A_HECHO: db TRUE ; Cambiar por `TRUE` para correr los tests.

; Marca el ejercicio 1B como hecho (`true`) o pendiente (`false`).
;
; Funciones a implementar:
;   - summon_fantastruco
global EJERCICIO_1B_HECHO
EJERCICIO_1B_HECHO: db TRUE ; Cambiar por `TRUE` para correr los tests.

;########### ESTOS SON LOS OFFSETS Y TAMAÑO DE LOS STRUCTS
; Completar las definiciones (serán revisadas por ABI enforcer):
DIRENTRY_NAME_OFFSET EQU 0
DIRENTRY_PTR_OFFSET EQU 16
DIRENTRY_SIZE EQU 24

FANTASTRUCO_DIR_OFFSET EQU 0
FANTASTRUCO_ENTRIES_OFFSET EQU 8
FANTASTRUCO_ARCHETYPE_OFFSET EQU 16
FANTASTRUCO_FACEUP_OFFSET EQU 24
FANTASTRUCO_SIZE EQU 32

; void init_fantastruco_dir(fantastruco_t* card[rdi]);
global init_fantastruco_dir
init_fantastruco_dir:
	push rbp
	mov rbp,rsp
	push rbx
	push r15
	;verfifquemos que el puntero no sea nulo 
	cmp rdi,0
	je .epilogo

	;si no es nulo entonces incializamos __dir y __dir__entries 
	mov rbx,rdi ;preservamos en un registro no volatil 

	;pedimos memoria para inicializar dir 
	mov rdi,DIRENTRY_SIZE
	shl rdi,1 ;multiplicamos por 2, porque tiene 2 elementos inicialmente 
	call malloc ;en rax tenemos el puntero 

	cmp rax,0
	je .epilogo ;si no tenemos memoria entonces saltamos al epilogo 

	;movemos rax un registro no volatil 
	mov r15,rax



	;ahora tenemos que cargar las habilidades 

	lea rdi,sleep_name
	lea rsi, [rel sleep] 
	call create_dir_entry
	;en rax tenemos el puntero lo ponemos en la primera posicion de __dir 
	mov [r15 + 0], rax 

	;ahora con la siguiente funcion 
	lea rdi,wakeup_name
	lea rsi, [rel wakeup]
	call create_dir_entry

	mov [r15 + 8], rax 

	;cargamos el dir en __dir 
	mov [rbx + FANTASTRUCO_DIR_OFFSET],r15 
	;ahora seteamos el __dir__entries en 2 
	mov [rbx + FANTASTRUCO_ENTRIES_OFFSET], word 2

	.epilogo:
	pop r15
	pop rbx
	mov rsp,rbp
	pop rbp
	ret 

; fantastruco_t* summon_fantastruco();
global summon_fantastruco
summon_fantastruco:
	push rbp
	mov rbp,rsp 
	push r15
	push r14

	;pedimos memoria con malloc 
	mov rdi,FANTASTRUCO_SIZE
	call malloc ;en rax tenemos el puntero

	;verificamos que no sea NULL 
	cmp rax,0 
	je .epilogo
	mov r15,rax 

	mov rdi,rax 
	call init_fantastruco_dir

	;tenemos el puntero modificado en r15 
	;seteamos el arquetipo y face_up 

	mov qword [r15 + FANTASTRUCO_ARCHETYPE_OFFSET],  0
	mov byte [r15 + FANTASTRUCO_FACEUP_OFFSET],  1

	mov rax,r15

	.epilogo: 
	pop r14
	pop r15
	mov rsp,rbp
	pop rbp
	ret 