# Datos importantes:

- Toda tarea pertenece como maximo a una pareja.
- Toda tarea puede abandonar su pareja.
- Las tareas pueden formar parejas cuantas veces quieran, pero solo pueden pertenecer a una en un momento dado.

- Las tareas de una pareja comparte un memoria de 4MB a partir de la direccion virtual `0xC0C00000`.
- Solo la tarea lider (que crea la pareja) puede escribir en la memoria compartida.
- Cuando una tarea abandona su pareja pierde acceso a los 4MB a partir de 0xC0C00000.

# Ejercicios:

## Parte 1: implementación de las syscalls (80 pts)

1. (50 pts) Definir el mecanismo por el cual las syscall **crear_pareja**, **juntarse_con** y **abandonar_pareja** recibirán sus parámetros y retornarán sus resultados según corresponda. Dar una implementación para cada una de las syscalls. Explicitar las modificaciones al kernel que sea necesario realizar, como pueden ser estructuras o funciones auxiliares.

Bueno tenemos que definir 3 syscalls, **crear_pareja**, **juntarse_con** y **abandonar_pareja**, como primer paso tenemos que definir las 3 entradas de la **idt** para poder vectorizar las nuevas syscalls. Lo hacemos de la siguiente manera:

```C
void idt_init()
{
//...
IDT_ENTRY3(90); // syscall crear_pareja
IDT_ENTRY3(91); // syscall juntarse_con
IDT_ENTRY3(92); // syscall abandonar_pareja
//...
}
```

La macro usada para la creacion de las entradas es la definida en el tp que tiene la siguiente implementacion:

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

A continucion debemos definir los handlers de la isr para atender las syscalls, lo que vamos a hacer es si alguna syscall nesecita parametros vamos a asumir que se pasan por **ECX** y si retorna algo lo va a hacer por **EAX**. Un ejemplo de uso seria

```ASM
int 90 ;para la syscall crear pareja

mov ecx, <Id de la tarea>
int 91 ;para la syscall juntarse_con

int 92; para la syscall abandonar pareja
```

En la isr vamos a definir la rutina de atencion de la syscall, vamos a usar funciones wrappers para abstraer la logica principal de la rutina de antencion a una funcion en C que se encargue:

```ASM
extern crear_pareja
extern juntarse_con
extern abandonar_pareja

global isr_90
global isr_91
global isr_92


isr_90:
    pushad

    call crear_pareja
    mov [esp+28], eax

    popad
    iret
isr_91:
    pushad

    push ecx
    call juntarse_con
    add esp,4

    mov [esp+28], eax ;Movemos el resultado al eax de la pila

    popad
    iret
isr_92:
    pushad

    call abandonar_pareja
    mov [esp+28], eax

    popad
    iret
```

Ademas debemos modificar algunas estructuras ya existentes del **KERNEL** original del tp, debido a que tenemos mas estados en el contexto de una tarea y debe ser conocido por el shceduler. Vamos a modificar la siguiente estructura:

En **sched.c**

```C
typedef struct
{
  int16_t selector;
  task_state_t state;
  //Nuevos atributos

  uint8_t id_pareja; //Para saber cual es la pareja de esta
  bool es_lider;  // Para saber si es lider.
} sched_entry_t;

//Vamos a agregar 2 estados
typedef enum
{
  TASK_SLOT_FREE,
  TASK_RUNNABLE,
  TASK_PAUSED,
  TASK_ESPERANDO_PAREJA,
  TASK_LIDER_BLOQUEDA
} task_state_t;
```

Por ultimo para terminar de definir las syscalls tenemos que dar una implementacion de las funciones wrappers. Las cuales van a estar definidas en **mmu.c**:

```C
%define VIRT_PAREJA_ADDR 0xC0C00000
%define 4MB (4 * 1024 * 1024)
//Funcion con la logica principal de la syscall crear_parejas
void crear_pareja (void) {
    if (aceptando_pareja(current_task)) {
        conformar_pareja(0); //Inicializamos la pareja y la logica la dejamos a parte para esta funcion
    }
}

uint8_t juntarse_con(uint8_t id_task) {


    if(aceptando_pareja(id_task)) {
        conformar_pareja(id_task);
        return 0;
    }

    return 1;
}

void abandonar_pareja() {
    if (pareja_actual() == 0) return;

    paddr_t cr3 = selector_task_to_cr3(sched_tasks[current_task]);

    if (!es_lider(current_task)) {
        uint8_t lider_id = pareja_actual();

        romper_pareja(); // rompe el vínculo lógico

        for (int addr = VIRT_PAREJA_ADDR; addr < VIRT_PAREJA_ADDR + 4MB; addr++) {
            mmu_unmap_page(cr3, addr);
        }

        if (sched_tasks[lider_id].state == TASK_LIDER_BLOQUEADO) {
            sched_tasks[lider_id].state = TASK_RUNNABLE;
        } else {
            sched_tasks[lider_id].state = TASK_ESPERANDO_PAREJA;
            sched_next_task();
        }

    } else {
        // Soy líder: rompo la pareja, desmapeo y me bloqueo
        romper_pareja();

        for (int addr = VIRT_PAREJA_ADDR; addr < VIRT_PAREJA_ADDR + 4MB; addr++) {
            mmu_unmap_page(cr3, addr);
        }

        sched_tasks[current_task].state = TASK_LIDER_BLOQUEADO;
        sched_next_task();
    }
}


```

Como la asignacion de meoria es on-demand esto quiere decir que se va a mapear cuando surga un page_fault_handler para que tenga en cuenta esta nueva direccion virtual.

```C
bool page_fault_handler(vaddr_t virt)
{
  print("Atendiendo page fault...", 0, 0, C_FG_WHITE | C_BG_BLACK);
  // Chequeemos si el acceso fue dentro del area on-demand
  // En caso de que si, mapear la pagina

  if (virt >= ON_DEMAND_MEM_START_VIRTUAL &&
      virt <= ON_DEMAND_MEM_END_VIRTUAL)
  {
    uint32_t cr3 = rcr3();
    mmu_map_page(cr3, ON_DEMAND_MEM_START_VIRTUAL, ON_DEMAND_MEM_START_PHYSICAL,
                 MMU_P | MMU_W | MMU_U);
    return true;
  }

  //Si llega un page fault tenemos que verificar que esta en el rango
  paddr_t phy_addr = mmu_next_free_user_page();
  if (virt >= VIRT_PAREJA_ADDR && virt < VIRT_PAREJA_ADDR + 4MB) {
    if (es_lider(current_task)) {
        //Si la tarea actual es lider, entonces puede escribir
        mmu_map_page(rcr3(),virt,phy_addr,MMU_P | MMU_W | MMU_U);
        mmu_map_page(selector_task_to_cr3(sched_tasks[actual_de_pareja()].selector),virt,phy,MMU_P | MMU_U);
    } else {
        // Si no es lider, mapeo a la pareja de la actual para que pueda escribir.
        mmu_map_page(rcr3(),virt,phy_addr,MMU_P | MMU_U);
        mmu_map_page(selector_task_to_cr3(sched_tasks[actual_de_pareja()].selector),virt,phy,MMU_P | MMU_U | MMU_W);
    }
    zero_page(virt & (0xfffff000)); //Zereamos la pagina asignada.
    return true;
  }
  return false;
}
```

Ahora vamos a dar la implementacion de la funcion auxiliar dada para conseguir el cr3 de una tarea a partir de su selector, lo que haces buscar en la tss de la tarea el cr3:

```C
paddr_t selector_task_to_cr3 (uint16_t selector) {
    //Nos pasan un selector lo shifteo 3 veces a la derecha

    //Lo shifteo para obtener el indice de la gdt
    uint32_t gdt_index = (uint32_t)(selector >> 3);
    //a partir de eso obtengo el descriptor de la tss
    gdt_entry_t tss_descriptor = gdt[gdt_index];

    vaddr_t base_tss = (tss_descriptor.base_31_24 << 24)| (tss_descriptor.base_23_16 << 16) |(tss_descriptor.base_15_0);
    //armo la base de la tss
    tss_t* tss_target = (tss_t*)(base_tss);

    //Extraigo el campo del cr3
    paddr_t cr3 = tss_targer->cr3;

    return cr3;
}
```

2. (20 pts) Implementar `conformar_pareja(task_id tarea)` y `romper_pareja()`

Vamos primero con la implementacion de `conformar_pareja(task_id tarea)`, el funcionamiento de la siguiente:

- Si task_id == 0 ponemos a la tarea actual como buscando pareja y lider
- si task_id != 0 ponemos a la tarea actual como lider, la id de la pareja como task_id, a la paraje pasada por parametro como no-lider y la id de la actual en la paraje.

```C
void conformar_pareja(task_id tarea) {
    sched_entry_t *current = &sched_tasks[current_task];
    current->lider = true;
    if( tarea == 0) {

        current->id_pareja = 0;
        current->state =TASK_ESPERANDO_PAREJA;
    } else {

        sched_entry_t* pareja = &sched_tasks[tarea];
        current->state = TASK_RUNNABLE;
        current->id_pareja = tarea;

        pareja->lider = false;
        pareja->state = TASK_RUNNABLE;
        pareja->id_pareja = current_task;
    }

}
```

Ahora seguimos con `romper_pareja()` el funcionamiento de la misma deberia ser el siguiente, asumiendo que esta en pareja (si no no pasa nada):

- Si la tarea actual es no-lider entonces a ambas tareas le pongo 0 en la id_pareja, y la pareja lider lo sigue siendo, la no-lider puede correr con normalidad
- Si la tarea actual es lider entonces no puedo romper el vinculo, de eso se encarga

```C
void romper_pareja(void) {
    uint16_t pareja = pareja_actual();
    if (pareja == 0) return;  // nada que romper

    if (!es_lider(current_task)) {
        // caso no-líder: limpia ambos estados
        sched_tasks[pareja].pareja_actual = 0;
        sched_tasks[pareja].es_lider = true; //la pareja siguie siendo lider en general no de esta pareja

        sched_tasks[current_task].pareja_actual = 0;
        sched_tasks[current_task].es_lider = false; // No era lider lo dejamos como estaba
        sched_tasks[current_task].state = TASK_RUNNABLE;
    } else {
        // caso líder: no rompe el vínculo, solo marca que ya no es líder activo
        // (el vínculo se romperá definitivamente cuando la no-líder abandone)
        sched_tasks[current_task].es_lider = 1; // sigue siendo líder
        // no se toca pareja_actual
    }
}

```

3. (10 pts) Implementar `pareja_de_actual()`, `es_lider(task_id tarea)` y `aceptando_pareja(task_id tarea)`

Vamos primero con `pareja_de_actual()` la implementacion es sencilla:

```C
uint8_t pareja_de_actual(void) {
    return sched_tasks[current_task].id_pareja; //si es 0 devulve 0, si es otra cosa devuelve la id de la pareja
}
```

Seguimos con `es_lider(uint8_t tarea_id)`:

```C
bool es_lider(uint8_t tarea_id) {
    return sched_tasks[tarea_id].es_lider;
}
```

Por ultimo con la implementacion de `aceptando_pareja(uint8_t task_id)`:

```C
bool aceptando_pareja(uint8_t task_id) {
    if (task_id == 0) return false;
    sched_entry_t task = sched_tasks[task_id];
    if (task.state == TASK_ESPERANDO_PAREJA) return true;

    return false;
}
```

## Parte 2: monitoreo de la memoria de las parejas (20 pts)

4.
