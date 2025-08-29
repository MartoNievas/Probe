# Introducción

Los ejercicios consisten en modificar el código de cada función para que pase todos los tests (sin errores de ABI ni Valgrind) usando como base el código provisto (sin empezar desde cero).

Cada ejercicio tiene la descripción del comportamiento esperado de la función, su declaración y el código en asm para corregir.

Es recomendable tener a mano la cheatsheet de Convención C y el machete de GDB (https://macapiaggio.github.io/gdb-guide).

# Ejercicio 1

Devuelve la suma  de los 5 sumandos

```c
uint64_t ejercicio1(uint64_t sum1, uint64_t sum2, uint64_t sum3, uint64_t sum4, uint64_t sum5);
```

```nasm
ejercicio1:
	add edi, ecx
	add edi, edx
    add edi, ebx
    add edi, r9d
	mov eax, edi
	ret
```

# Ejercicio 2

Recibe un puntero a un struct de item, un id, una cantidad y un nombre. Copia los 3 datos usando el puntero al struct item.

```c
void ejercicio2(item_t* un_item, uint32_t id, uint32_t cantidad, char nombre[]);
```

```nasm
ITEM_OFFSET_NOMBRE EQU 9
ITEM_OFFSET_ID EQU 16
ITEM_OFFSET_CANTIDAD EQU 24

ejercicio2:
	mov [rdi+ITEM_OFFSET_ID], rsi
	mov [rdi+ITEM_OFFSET_CANTIDAD], rdx 
	call strcpy
	ret
```

```c
typedef struct {
	char nombre[9];
	uint32_t id;
	uint32_t cantidad;
} item_t;
```

# Ejercicio 3

Recibe un puntero a un array de uint32\_t arr, un tamaño n y un puntero a una función fun. Devuelve:

$$
ej3(arr,n,fun) =\begin{cases}
			64 & \text{si $n = 0$}\\
            fun(0,arr[0]) & \text{si $n = 1$}\\
            ej3(arr,n-1,fun) + fun(ej3(arr,n-1,fun),arr[n-1]) & \text{si $n > 1$}
		 \end{cases}
$$

```c
uint32_t ejercicio3(uint32_t* array, uint32_t size, uint32_t (*fun_ej_3)(uint32_t a, uint32_t b));
```

```nasm
ejercicio3:
	cmp rsi, 0
	je .vacio
	
	mov rcx, rdi ; array
	mov r8, 0 ; resultado parcial
	mov r9, 0 ; i

	.loop:
	mov rdi, r8
	mov rsi, [rcx + r9*4]

	call rdx

	add r8, rax
	mov rax, r8

	inc r9
	cmp r9, rsi
	je .end

	jmp .loop

	.vacio:
	mov rax, 64

	.end:
	ret
```

# Ejercicio 4

Recibe un puntero a un array de punteros a uint32_t, un tamaño y una constante C. Devuelve un puntero a un array del mismo largo donde cada entero está multiplicado por la constante C.

Hay que liberar las direcciones de memoria donde se guarda cada entero y ponerlas en NULL. Asumir que el array siempre tiene al menos un elemento. Si está vacío el comportamiento es indefinido. 

Ejemplo:

![Ejemplo ejercicio 4](../../img/debuggingEj4punterosArray.png)

```c
uint32_t* ejercicio4(uint32_t** array, uint32_t size, uint32_t constante);
```

```nasm
POINTER_SIZE EQU 4
UINT32_SIZE EQU 8

ejercicio4:
	mov r12, rdi
	mov r13, rsi
	mov r14, rdx

	xor rdi, rdi
	mov eax, UINT32_SIZE
	mul esi
	mov edi, eax

	call malloc
	mov r15, rax
	
	xor rbx, rbx
	.loop:
	
	cmp rbx, r13
	je .end

	mov r8, [r12+rbx*POINTER_SIZE]
	mov r9d, [r8]
	mov rax, r14
	mul r9d
	mov [r15+rbx*UINT32_SIZE], eax

        mov rsi, r8 
	call free

	inc rbx
	jmp .loop

	.end:
	mov rax, r15
	ret
```
