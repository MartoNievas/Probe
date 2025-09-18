# 📦 ABI System V AMD64 – Resumen para Parcial

## 📌 Parámetros por registro (enteros/punteros)

| Orden | Registro | Descripción                      |
|-------|----------|----------------------------------|
| 1     | rdi      | Primer parámetro                 |
| 2     | rsi      | Segundo parámetro                |
| 3     | rdx      | Tercer parámetro                 |
| 4     | rcx      | Cuarto parámetro                 |
| 5     | r8       | Quinto parámetro                 |
| 6     | r9       | Sexto parámetro                  |
| 7+    | stack    | Por la pila (`[rbp + offset]`)   |

---

## 🧐 Valores de retorno

| Tipo de dato      | Registro de retorno |
|-------------------|---------------------|
| bool / int8       | al                  |
| int32 / uint32    | eax                 |
| int64 / pointer   | rax                 |

---

## 📂 Registros: preservación (caller vs callee save)

### Caller-save (se pueden pisar con `call`):

- `rax`, `rcx`, `rdx`, `rsi`, `rdi`, `r8`, `r9`, `r10`, `r11`

### Callee-save (si los usás, tenés que preservarlos):

- `rbx`, `rbp`, `r12`, `r13`, `r14`, `r15`

---

## 🖐️ Subregistros por tamaño

| Registro base | 64 bits | 32 bits | 16 bits | 8 bits bajo |
|---------------|---------|---------|---------|-------------|
| rax           | rax     | eax     | ax      | al          |
| rbx           | rbx     | ebx     | bx      | bl          |
| rcx           | rcx     | ecx     | cx      | cl          |
| rdx           | rdx     | edx     | dx      | dl          |
| rsi           | rsi     | esi     | si      | sil         |
| rdi           | rdi     | edi     | di      | dil         |
| r8            | r8      | r8d     | r8w     | r8b         |
| r9            | r9      | r9d     | r9w     | r9b         |
| r10           | r10     | r10d    | r10w    | r10b        |
| r11           | r11     | r11d    | r11w    | r11b        |
| r12           | r12     | r12d    | r12w    | r12b        |
| r13           | r13     | r13d    | r13w    | r13b        |
| r14           | r14     | r14d    | r14w    | r14b        |
| r15           | r15     | r15d    | r15w    | r15b        |

---

## 🛠️ Operaciones úTiles frecuentes en ASM

# Conversión entre enteros, floats y doubles en x86-64 (SSE2+)

```asm
; ========================
; Entero → Float
; ========================
cvtsi2ss xmm1, r/m32   ; Convierte entero de 32 bits → float en xmm1
cvtsi2ss xmm1, r/m64   ; Convierte entero de 64 bits → float en xmm1 (x86-64)

; ========================
; Float → Entero
; ========================
cvttss2si r32, xmm1    ; Convierte float → entero de 32 bits
cvttss2si r64, xmm1    ; Convierte float → entero de 64 bits
; cvtt = convert with truncation (ignora el modo de redondeo MXCSR)

; ========================
; Entero → Double
; ========================
cvtsi2sd xmm1, r/m32   ; Convierte entero de 32 bits → double en xmm1
cvtsi2sd xmm1, r/m64   ; Convierte entero de 64 bits → double en xmm1 (x86-64)

; ========================
; Double → Entero
; ========================
cvttsd2si r32, xmm1    ; Convierte double → entero de 32 bits
cvttsd2si r64, xmm1    ; Convierte double → entero de 64 bits

; ========================
; Float → Double
; ========================
cvtss2sd xmm1, xmm2    ; Convierte float en xmm2 → double en xmm1

; ========================
; Double → Float
; ========================
cvtsd2ss xmm1, xmm2    ; Convierte double en xmm2 → float en xmm1


## Declarar un string con null terminator
```asm
section .data
    mi_cadena db "Hola, mundo!", 0 ; La cadena termina con un byte nulo (0x00)
```
### Acceder a un índice de array:
```asm
lea rax,[rdi] ;carga la direccion efectiva en rax, es como el & en C
movzx rax, word [rsi + rdx*2]    ; acceder a indice[i] de uint16_t*
mov rbx, [rdi + rax*8]           ; acceder a inventario[indice[i]] de item_t**
```

### Copiar un puntero a resultado:
```asm
mov [r14 + r13*8], rbx           ; resultado[i] = inventario[indice[i]]
```

### Copiar una estructura de 28 bytes (ej: item_t):
```asm
mov rax, [rsi]       ; 8 bytes
mov [rdi], rax
mov rax, [rsi+8]     ; siguiente 8
mov [rdi+8], rax
mov rax, [rsi+16]    ; siguiente 8
mov [rdi+16], rax
mov ax, [rsi+24]     ; últimos 2 bytes
mov [rdi+24], ax
```

### Comparar resultado booleano:
```asm
call rcx
; valor devuelto está en al

; chequear si es falso
test al, al
jz .no_ordenado
```

### Multiplicar por potencias de 2
```asm
shl rax, 1 ; x2
shl rax, 2 ; x4
shl rax, 3 ; x8
shl rax, 4 ; x16
```

---

## 📊 Convención de pila (stack frame)

```
| argumento 7+       | <- rsp al entrar (si hay más de 6 args)
| ------------------|
| return address     |
| rbp (viejo)        | <- rbp (marco de pila)
| variables locales  |
| registros guardados|
```
# 🐞 Guía rápida de GDB

## 📌 Iniciar GDB
```bash
gdb ./mi_programa
gdb ./mi_programa archivo_de_prueba
```

## 🟢 Comandos esenciales
```bash
run             # Ejecuta el programa
start           # Comienza ejecución y se frena en main
next / n        # Ejecuta siguiente línea (salta funciones)
step / s        # Entra en función si hay una llamada
continue / c    # Continúa hasta próximo breakpoint
finish          # Ejecuta hasta salir de la función actual
quit / q        # Salir de GDB
```

## ⛳ Breakpoints
```bash
break main
break archivo.c:42
break funcion
delete [número]      # Borra breakpoint
info breakpoints     # Lista breakpoints
```

## 🔍 Inspeccionar valores
```bash
print x              # Muestra el valor de x
print/x x            # Muestra en hexadecimal
print/d x            # Muestra en decimal
print/c x            # Muestra como caracter
display x            # Muestra el valor en cada paso automáticamente
undisplay n          # Deja de mostrar ese display
```

## 🧠 Estructuras y punteros
```bash
print *ptr           # Muestra la estructura apuntada por ptr
print ptr->campo     # Campo dentro de struct
print arr[i]         # Elemento i de un array
ptype variable       # Tipo de la variable
```

## 📦 Memoria y punteros
```bash
x/4xg ptr            # Examina 4 valores de 8 bytes en hex desde ptr
x/s ptr              # Muestra string en ptr
x/20xb &var          # Muestra 20 bytes desde dirección de var
```

## 📄 Ver código fuente
```bash
list                 # Muestra las líneas del archivo fuente
list 20              # Muestra desde línea 20
list funcion         # Muestra el código de la función
```

## 🧮 Registros (ASM)
```bash
info registers       # Muestra todos los registros
print $rax           # Valor de un registro
```

## 🧱 Stack y backtrace
```bash
backtrace            # Muestra el call stack
frame n              # Cambia a otro frame
info frame           # Muestra info del frame actual
```

## 💡 Tips útiles
- Siempre compilar con `-g` para que GDB tenga símbolos:
```bash
gcc -g programa.c -o programa
```

- Podés usar expresiones como en C:
```bash
print a + b
print strcmp(str1, str2)
```
```

### 🔹 Código en `main.c`
```c
int main() {
    templo t1 = { 8, "Partenón", 3 };
    return 0;
}
```

### 🔹 Compilar con símbolos
```bash
gcc -g main.c -o templo_test
```

### 🔹 GDB paso a paso
```gdb
gdb ./templo_test
break main
run
print t1                     # Muestra todos los campos
print t1.colum_largo         # Campo específico
print t1.colum_corto
print t1.nombre              # Dirección del string
x/s t1.nombre                # Muestra el contenido del string
```

---

## 🧪 Ejemplo 2: Array de estructuras `templo`

### 🔹 Código en `main.c`
```c
int main() {
    templo templos[2] = {
        { 8, "Partenón", 3 },
        { 6, "Erecteion", 2 }
    };
    return 0;
}
```

### 🔹 GDB paso a paso
```gdb
gdb ./templo_test
break main
run
print templos[0]             # Muestra el primer struct
print templos[1]             # Muestra el segundo struct
print templos[1].nombre
x/s templos[1].nombre
```

---

## 🧪 Ejemplo 3 (Bonus): Puntero a estructura

### 🔹 Código en `main.c`
```c
int main() {
    templo t = { 9, "Templo de Zeus", 4 };
    templo *ptr = &t;
    return 0;
}
```

### 🔹 GDB paso a paso
```gdb
gdb ./templo_test
break main
run
print *ptr                   # Muestra todos los campos de la estructura
print ptr->nombre            # Dirección del nombre
x/s ptr->nombre              # String apuntado por el campo nombre
```
