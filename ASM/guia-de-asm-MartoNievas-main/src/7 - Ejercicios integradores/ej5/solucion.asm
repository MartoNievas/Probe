; Definiciones comunes
TRUE  EQU 1
FALSE EQU 0

; Identificador del jugador rojo
JUGADOR_ROJO EQU 1
; Identificador del jugador azul
JUGADOR_AZUL EQU 2

; Ancho y alto del tablero de juego
tablero.ANCHO EQU 10
tablero.ALTO  EQU 5

; Marca un OFFSET o SIZE como no completado
; Esto no lo chequea el ABI enforcer, sirve para saber a simple vista qué cosas
; quedaron sin completar :)
NO_COMPLETADO EQU -1

extern strcmp

;########### ESTOS SON LOS OFFSETS Y TAMAÑO DE LOS STRUCTS
; Completar las definiciones (serán revisadas por ABI enforcer):
carta.en_juego EQU 0
carta.nombre   EQU 1
carta.vida     EQU 14
carta.jugador  EQU 16
carta.SIZE     EQU 18

tablero.mano_jugador_rojo EQU 0
tablero.mano_jugador_azul EQU 8
tablero.campo             EQU 16
tablero.SIZE              EQU 416

accion.invocar   EQU 0
accion.destino   EQU 8
accion.siguiente EQU 16
accion.SIZE      EQU 24

; Variables globales de sólo lectura
section .rodata

; Marca el ejercicio 1 como hecho (`true`) o pendiente (`false`).
;
; Funciones a implementar:
;   - hay_accion_que_toque
global EJERCICIO_1_HECHO
EJERCICIO_1_HECHO: db TRUE

; Marca el ejercicio 2 como hecho (`true`) o pendiente (`false`).
;
; Funciones a implementar:
;   - invocar_acciones
global EJERCICIO_2_HECHO
EJERCICIO_2_HECHO: db TRUE

; Marca el ejercicio 3 como hecho (`true`) o pendiente (`false`).
;
; Funciones a implementar:
;   - contar_cartas
global EJERCICIO_3_HECHO
EJERCICIO_3_HECHO: db TRUE

section .text

; Dada una secuencia de acciones determinar si hay alguna cuya carta tenga un
; nombre idéntico (mismos contenidos, no mismo puntero) al pasado por
; parámetro.
;
; El resultado es un valor booleano, la representación de los booleanos de C es
; la siguiente:
;   - El valor `0` es `false`
;   - Cualquier otro valor es `true`
;
; ```c
; bool hay_accion_que_toque(accion_t* accion, char* nombre);
; ```
global hay_accion_que_toque
; Te recomendamos llenar una tablita acá con cada parámetro y su
	; ubicación según la convención de llamada. Prestá atención a qué
	; valores son de 64 bits y qué valores son de 32 bits o 8 bits.
	;
	; r/m64 = accion_t*  accion [rdi]
	; r/m64 = char*      nombre [rsi]
hay_accion_que_toque:
	push rbp 
	mov rbp,rsp 
	push rbx 
	push r15 
	push r14
	push r13 
	push r12 
	sub rsp,8

	xor rax,rax ;seteamos el valor de res en false 
	;verficamos que ningun puntero sea null
	cmp rsi,0
	je .epilogo
	cmp rdi,0
	je .epilogo

	;caso contario 
	mov rbx, rdi 
	mov r15, rsi 
	.ciclo: 
	cmp rbx,0 
	je .caso_false
	;caso contrario verificamos 

	mov r14,[rbx + accion.destino]
	lea rdi, [r14 + carta.nombre]
	mov rsi,r15
	call strcmp 
	cmp rax,0 
	jne .next 

	mov rax,1
	jmp .epilogo

	.next: 
	mov rbx,[rbx + accion.siguiente] ;curr = curr->siguiente
	jmp .ciclo
	
	.caso_false:	
	xor rax,rax ;correcion fallaba que cualquier valor distinto de 0 era true y cada vez que llamo a strcmp se modifica rax

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

; Invoca las acciones que fueron encoladas en la secuencia proporcionada en el
; primer parámetro.
;
; A la hora de procesar una acción esta sólo se invoca si la carta destino
; sigue en juego.
;
; Luego de invocar una acción, si la carta destino tiene cero puntos de vida,
; se debe marcar ésta como fuera de juego.
;
; Las funciones que implementan acciones de juego tienen la siguiente firma:
; ```c
; void mi_accion(tablero_t* tablero, carta_t* carta);
; ```
; - El tablero a utilizar es el pasado como parámetro
; - La carta a utilizar es la carta destino de la acción (`accion->destino`)
;
; Las acciones se deben invocar en el orden natural de la secuencia (primero la
; primera acción, segundo la segunda acción, etc). Las acciones asumen este
; orden de ejecución.
;
; ```c
; void invocar_acciones(accion_t* accion[rdi], tablero_t* tablero[rdi]);
; ```


global invocar_acciones
invocar_acciones:
	push rbp 
	mov rbp,rsp 
	push rbx 
	push r15 
	push r14 
	push r13
	push r12 
	sub rsp,8

	;verificamos que ningun puntero sea nulo 
	cmp rdi,0
	je .epilogo
	cmp rsi,0
	je .epilogo

	;caso contrario invocamos acciones 
	;preservamos el puntero a la accion y el puntero al tablero en registro no volatiles 
	mov rbx,rdi ;aqui tenemos la accion 
	mov r15,rsi ;aqui tenemos el tablero 

	.ciclo: 
	cmp rbx,0
	je .epilogo
	;caso contrario iteramos 

	mov r14,[rbx + accion.destino]
	mov r13b, byte [r14 + carta.en_juego] ; en r13b tenemos si esta en juego la carta 
	cmp r13b,1
	jne .next ;si no esta en juego seguimos iterando 

	;caso contrario verifcamos si la vida es 0, con la carta en r14 
	movzx r12, word [r14 + carta.vida] ;en r12b tenemos la vida 
	cmp r12,0
	je .caso_cero_vida_desde_el_incio

	;caso contrario tiene vida > 0 entonce solo tenemos que invocar la funcion y verficar si despues de la ejecucion tiene 0 o mas vida 
	mov rdi,r15
	mov rsi,[rbx + accion.destino]
	call [rbx + accion.invocar] ;llamamos a la funcion 
	;ahora verfifcamos si la vida es 0, si es 0 tenemos que dejarla fuera de juego 

	cmp [r14 + carta.vida],byte 0
	jne .next 

	.caso_cero_vida_despues_de_invocar: 
	mov [r14 + carta.en_juego], byte 0
	jmp .next

	.caso_cero_vida_desde_el_incio: 
	mov rdi,r15
	mov rsi,[rbx + accion.destino]
	call [rbx + accion.invocar]
	mov [r14 + carta.en_juego], byte 0

	.next:
	mov rbx,[rbx +accion.siguiente]
	jmp .ciclo


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

; Cuenta la cantidad de cartas rojas y azules en el tablero.
;
; Dado un tablero revisa el campo de juego y cuenta la cantidad de cartas
; correspondientes al jugador rojo y al jugador azul. Este conteo incluye tanto
; a las cartas en juego cómo a las fuera de juego (siempre que estén visibles
; en el campo).
;
; Se debe considerar el caso de que el campo contenga cartas que no pertenecen
; a ninguno de los dos jugadores.
;
; Las posiciones libres del campo tienen punteros nulos en lugar de apuntar a
; una carta.
;
; El resultado debe ser escrito en las posiciones de memoria proporcionadas
; como parámetro.
;
; ```c
; void contar_cartas(tablero_t* tablero[rdi], uint32_t*[rsi] cant_rojas, uint32_t* cant_azules[rdx]);
; ```
global contar_cartas
contar_cartas:
	push rbp
	mov rbp,rsp 
	push rbx 
	push r15
	push r14 
	push r13
	push r12 
	sub rsp,8

	;verficamos que los punteros no sean nulos 
	cmp rdi,0
	je .epilogo
	cmp rsi,0
	je .epilogo
	cmp rdx,0 
	je .epilogo

	;caso contrario pasamos los parametros de entrada a registros no volatiles 
	mov rbx,rdi 
	mov r15,rsi 
	mov r14,rdx 

	;ponemos el campo del tablero en r13 
	lea r13, [rbx + tablero.campo] ;aqui tenemos un puntero al incio del campo
	
	;inicializamos los contadores de cartas 
	xor r9,r9 ;cant jugador rojo 
	xor r10,r10 ;cant jugador aazul
	xor r11,r11 ;int i = 0; 
	.ciclo: 
	cmp r11,50
	jge .fin_ciclo
	;ponemos la carta en r12 
	mov r12, [r13 + r11*8]

	;ahora verficamos que el puntero no sea null 
	cmp r12,0 
	je .next 

	;caso contrario debemos verificar si es rojo o azul 

	cmp [r12 + carta.jugador],byte JUGADOR_ROJO
	jne .probar_jugador_azul 
	;si es jugador rojo 
	inc r9 
	jmp .next


	.probar_jugador_azul:	
	cmp [r12 + carta.jugador], byte JUGADOR_AZUL
	jne .next

	;si es jugador azul 
	inc r10 

	.next: 
	inc r11 
	jmp .ciclo

	.fin_ciclo:
	mov [r15], r9d
	mov [r14], r10d



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
