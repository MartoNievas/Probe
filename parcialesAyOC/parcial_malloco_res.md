# Ejercicio 1: (50 puntos)

Detallar todos los cambios que es necesario realizar sobre el kernel para que una tarea de nivel usuario pueda pedir memoria y liberarla, asumiendo como ya implementadas las funciones mencionadas. Para este ejercicio se puede asumir que la tarea 0 está implementada y funcionando.

## Respuesta:

Primero que nada `malloco` es una syscall, con lo cual va a poder ser llamada por tareas de usuario, pero en el fondo va a ser antendida por el kernel, comenzemos definiendo una nueve entrada en la **IDT**:

```C
IDT_ENTRY(90)
```

Donde la definicion de **IDT_ENTRY3** es:

```C
#define IDT_ENTRY3(numero)                         \
  idt[numero] = (idt_entry_t){                     \
      .offset_31_16 = HIGH_16_BITS(&_isr##numero), \
      .offset_15_0 = LOW_16_BITS(&_isr##numero),   \
      .segsel = GDT_CODE_0_SEL,                    \
      .type = INTERRUPT_GATE_TYPE,                 \
      .dpl = 0x3,                                  \
      .present = 0x01}
```

Como podemos observar el dpl es 0x3 indica que puede ser llamada por un tarea de nivel usuario, lo cual es exactamente lo que queremos. Con lo cual se podria utilizar de la siguiente manera:

```ASM
mov eax, <Cantidad de bytes a allocar>
int 90
```

Ahora debemos implementar el handler que se ocupa del manejo de la interrupccion que va a ser el siguiente:

```ASM
;malloco
global _isr90

_isr90:

pushad

push eax
call malloco_c

add esp,4

popad
```
