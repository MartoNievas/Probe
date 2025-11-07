# Datos Importantes:

- 1. Tenemos un lector de cartuchos con un buffer de video de 4 kb (1 pagina).
- 2. Cada vez que el buffer se llene y tenga algun modo de indicar que esta listo va a informar al kernel mediante una interrupcion mapeada a IRQ 40.
- 3. Nuevos mecanimos donde debe acceder el buffer:
  - DMA (Direct Memory Access): se mappea la dirección virtual 0xBABAB000 del espacio de direcciones de la tarea directamente al buffer de video (Para todas las tareas que accedan por DMA).
  - Por copia: se realiza una copia del buffer en una página física específica y se mapea en una dirección virtual provista por la tarea. Cada tarea debe tener una copia única (Por copia puede elegir donde mapiarse en memoria fisica y virtual).
- 4. El buffer se encuentra en la dir de memoria fisica 0xF151C000 solo se puede modificar por el lector de cartuchos.
- 5. Variable para saber que tipo de acceso 0 si la tarea no accede al buffer, 1 por DMA y 2 por por copia.
- 6. De acceder por copia, la direccion virtual de la copia estara en el registro **ECX**

# Ejercicio 1:

- a) Programar la rutina que atenderá la interrupción que el lector de cartuchos generará al terminar de llenar el buffer.

  - Consejo: programar una función deviceready y llamarla desde esta rutina.

- b) Programar las syscalls opendevice y closedevice.

- Cuentan con las siguientes funciones ya implementadas:

  - void buffer_dma(pd_entry_t\* pd) que dado el page directory de una tarea realice el mapeo del buffer en modo DMA.
  - void buffer_copy(pd_entry_t\* pd, paddr_t phys, vaddr_t virt) que dado el page directory de una tarea realice la copia del buffer a la dirección física pasada por parámetro y realice el mapeo a la dirección virtual pasada por parámetro.

  ## Respuestas:

1.  a) Lo primero que debemos hacer es implementar la intrrupccion para el lector de cartuchos, lo que deberiamos a ser en grander rasgos es poner una nueva entrada en IDT para la rutina de antencion de la interrupccion del lector de cartuchos.

### Estructuras a Modificar :

- **TASK_STATE**: Vamos a agregar un nuveo estado de tarea para poder representar el estado **BLOCKED** y otro **KILLED** para mas adelante
- **IDT**: Tenemos que agregar una interrupcion por hardware y 2 syscalls.
- **SCHED_ENTRY** agregamos dos campos nuevos en este strcut, **MODE** indica el modo de acceso al buffer de video de los cartuchos y **copyDir** el cual es la direccion virtual de la copia de la pagina

Quedaria de la siguiente manera:

Nuevos estados para las tareas en **sched.c**

```C
typedef enum {
    TASK_SLOT_FREE,
    TASK_RUNNABLE,
    TASK_PAUSED,
    BLOCKED,
    KILLER          }  task_state_t;

```

```C
typedef struc {
    uint16_t selector;
    task_state state;

    //Agregamos para resolver
    //el mecanismo
    vaddr_t copyDir;
    uint8_t mode;
} sched_entry_t;

//En la idt no quedaria no siguiente:

void idt_init() {
    // ...
    IDT_ENTRY0(40);
    // ...
    IDT_ENTRY3(90);
    IDT_ENTRY3(91);
    // ...
}
```

Donde la defincion de `IDT_ENTRY0` y `IDT_ENTRY3` estan dadas por el tp y son la siguientes:

```C
#define IDT_ENTRY0(numero)                         \
  idt[numero] = (idt_entry_t){                     \
      .offset_31_16 = HIGH_16_BITS(&_isr##numero), \
      .offset_15_0 = LOW_16_BITS(&_isr##numero),   \
      .segsel = GDT_CODE_0_SEL,                    \
      .type = INTERRUPT_GATE_TYPE,                 \
      .dpl = 0x0,                                  \
      .present = 0x1}

#define IDT_ENTRY3(numero)                         \
  idt[numero] = (idt_entry_t){                     \
      .offset_31_16 = HIGH_16_BITS(&_isr##numero), \
      .offset_15_0 = LOW_16_BITS(&_isr##numero),   \
      .segsel = GDT_CODE_0_SEL,                    \
      .type = INTERRUPT_GATE_TYPE,                 \
      .dpl = 0x3,                                  \
      .present = 0x01}
```

En tasks.c modificamos lo siguiente:

```C
typedef enum {
    NO_ACCESS_MODE 0
    DMA_MODE 1;
    COPY_MODE 2;
}
```

Vamos con deviceready, el cual estaria ubicado en idt.c, La logica va a ser la siguiente, recorremos la lista de tareas del scheduler, si encontramos una bloqueda, entonces verficamos que modo de acceso tiene y mapeamos y la dejamos como runnable, en caso de que no este bloqueda actualizamos la pagina si esta por copia.

```C
void deviceready(void) {
    for (int i = 0; i < MAX_TASKS + 1; i++) {
        sched_entry_t current_task = sched_tasks[i];
        if (current_task.state == BLOCKED) {
            if (current_task.mode == NO_ACCESS) continue;

            if(current_task.state == ACCESS_DMA) {
                // La macro CR3_TO_PAGE_DIR la definimos en el tp,task_selector_to_cr3 es una funcion aux que vamos a definir
                buffer_dma(CR3_TO_PAGE_DIR(task_selector_to_cr3(current_task.selector)));
            }

            if (current_task.mode == ACCESS_COPY ) {
                buffer_copy(CR3_TO_PAGE_DIR(task_selector_to_cr3(current_task.selector)),
                mmu_next_user_page(),current_task.copyDir);
            }
            current.state = TASK_RUNNABLE;
        } else {
            if (current_task.mode == ACCESS_COPY) {
                paddr_t dest = virt_to_phy(task_selector_to_cr3(current_task.selector),
                current_task.copyDir);
                //Esa direccion es la del buffer (siempre copya en el mismo);
                copy_page((paddr_t)0xF151C000,dest);
            }
        }
    }
}
```

Tambien debemos modificar la isr, con la nueva interrupcion de hardware y las 2 syscalls

```ASM
extern deviceready

global isr_40

isr_40:
    pushad
    call pic_finish2
    call deviceready
    popad
    iret
```

Por ultimo debemos implementar las syscalls. Vamos a implementarla a travez de asembler en la isr, pero la logica principal va a estar en C. Ambas van a estar ubicadas en tasks.c

```ASM
extern opendevice
extern closedevice
global isr_91
global isr_90

isr_90:
    pushad

    //como en ecx recibimos la direccion virtual
    push ecx
    call opendevice
    add esp,4

    ;Ahora debemos hacer la conmutacion de tareas debido a opendevice nos bloquea la current task
    call sched_next_task

    str cx
    cmp ax, cx
    je .fin

    mov word [sched_task_selector], ax
    jmp far [sched_task_offset]

    popad
    iret


isr_91:
    pushad

    call closedevice

    popad
    iret

```

Ahora vamos a describir la logica de las funciones wrapper de las rutinas de atencion de la interrupcion. Ambos wrappers van a estar ubicados en **idt.c**. Vamos a hacer uso de una varible global en **sched.c** la cual es current_task la cual indica la tarea que se esta ejecutando acutualment.

```C
//Wraper de la syscall 90
void opendevice(vaddr_t copyDir) {
    sched_tasks[current_task].state = BLOCKED;
    sched_tasks[current_task].mode = *(uint8_t*)0xACCE5000;
    sched_tasks[current_task].copyDir = copyDir;
}

//Wrapper de la syscall 91
void closedevice(void) {
    //Lo que tengo que hacer es desmapear si el modo es DMA o dejar de copiar si el modo es copy
    if (sched_tasks[current_task].mode == ACCESS_DMA) {
        mmu_unmap_page(rcr3(),sched_tasks[current_task].copyDir);
    }
    //tengo que desmapear la pagina que se pidio
    if (sched_tasks[current_task].mode == ACCESS_COPY) {
        sched_tasks[current_task] = NO_ACCESS_MODE;
    }
}
//Preguntar
```

Ahora vamos con las definiciones de las funciones auxiliares que utilizamos anteriormente`task_selector_to_cr3`, lo que debemos hacer es partir del selector buscar la entrada en la **GDT**, luego con ese descriptor buscar la **TSS**:

```C
paddr_t task_selector_to_cr3 (uint16_t selector) {
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

Ahora vamos con la funcion auxiliar virt_to_phy la cual recibe un cr3 y una direccion virtual y tenemos que encontrar la direccion fisica asociada a esa direccion virtual, vamos a asumir que ya esta todo mapeado y tienen el bit de presente.

```C
paddr_t  virt_to_phy(paddr_t cr3, vaddr_t virt) {
    //Ambas macros definidas en el tp;
    uint32_t pd_index = VIRT_PAGE_DIR(virt);
    uint32_t pt_index = VIRT_PAGE_TABLE(virt);

    //Ahora tenemos que buscar la pd y la pt, la macro fue definida en el tp
    pd_entry_t* pd = (pd_entry_t*)(CR3_TO_PAGE_DIR(cr3));

    //ahora conseguimos la pt

    pt_entry_t* pt = (pt_entry_t*)(MMU_ENTRY_PADDR(pd[pd_index].pt));


    //Conseguimos la direccion fisica de la pagina, la macro fue definida en el tp
    paddr_t phy_add = MMU_ENTRY_PADDR(pt[pt_index].page);

    return phy_add;
}
```

# Ejercicio 2:

- a) Programar la función void buffer_dma(pd_entry_t\* pd)
- b) Programar la función void buffer_copy(pd_entry_t\* pd, paddr_t phys, vaddr_t virt)

## Respuesta:

a) Vamos con la implementacion de `void buffer_dma(pd_entry_t\* pd)`, por enunciado sabemos que el buffer esta en la direccion fisica **0xF151C000** y siempre lo mapeamos a la direccion virtual **0xBABAB000** y la funcion toma una **PAGE_DIRECTORY** por lo tanto vamos con la implementacion

```C
#define DMA_PHY_DIR (0xF151C000)
#define DMA_VIRT_DIR (0xBABAB000)


void buffer_dma(pd_entry_t* pd) {
    //Podemos obtener el cr3 ya que es la direccion del pd

    paddt_t cr3 = &pd;

    mmu_map_page(cr3,DMA_VIRT_DIR, DMA_PHY_DIR, MMU_P); //tiene que ser read-only
}

//Ahora vamos con buffer_copy

 void buffer_copy(pd_entry_t* pd, paddr_t phys, vaddr_t virt){
    //Este es nuestro cr3
    paddr_t cr3 = &pd;

    copy_page(phys,(paddr_t)0xF151C000); //Copiamo la pagina del buffer
    //Luego la mapeamos, podria hacerse al revez es indistinto.

    mmu_map_page(cr3,virt,phys,MMU_P | MMU_W); //Se debe poder escribir para mantener la copia actualizada

}
```

# EXTRA: ENUNCIADO

Nuestro kernel "Orga Génesis" es funcional, pero ineficiente. Hemos detectado que algunas tareas de Nivel 3
solicitan acceso al dispositivo (opendevice) pero luego **nunca leen los datos**.

Esto es un desperdicio crítico de recursos. En particular:

- En **Modo Copia**, se gastan 4KB de RAM valiosa para una copia que no se usa.
- El `deviceready` (IRQ 40) gasta ciclos de CPU actualizando copias.

Para solucionar esto, implementaremos una nueva tarea de **nivel 0** llamada task_killer. El codigo de esta tarea
reside en el area de kernel, se ejecutará como las demas tareas en round robin y debera deshabilitar cualquier
tarea de nivel 3 que haya desperdiciado recursos por mucho tiempo

Una tarea se considera ociosa y debe ser deshabilitada si cumple todas las siguientes condiciones:

    1. Ha estado en estado TASK_RUNNABLE por más de 100 "ticks" de reloj.
    2. Tiene un acceso activo al dispositivo (es decir, task[i].mode != NO_ACCESS y task[i].status !=
        TASK_BLOCKED).
    3. No ha leído la memoria del buffer (DMA o Copia) desde la última vez que el "killer" la inspeccionó

3. Describa todos los pasos para crear e inicializar la nueva tarea task_killer:
   Detalle los cambios necesarios en:

## Modificaciones en las estrcuturas:

- 1. **GDT** Como tenemos una nueva tarea vamos a agregar un nuevo descriptor de **TSS**, pero como es de nivel 0 debemos hacer un par de modificaciones en el la tabla **TSS** y ademas en como inicializamos la tarea:

- 2. Nueva entrada de tarea en el scheduler

- 3. Agregar una estructura adiccional para poder llevar un conteo de los ticks en las tareas cuando estan en estado **RUNNABLE**

Empezemos por las nuevas entradas en la **GDT** entonces tendriamos que hacer lo siguiente:
Estos son los cambios a realizar en `tss.c` para poder crear una tarea de nivel 0, como estamos en nivel de kernel solo tenemos una pila, por eso el esp = esp0. Tambien tenemos que definir la nueva funcion `mmu_init_killer_task_dir(vaddr_t code_start)` para mapear la paginas de codigo y el identity maping del kernel.

```C
tss_t tss_create_killer_task(paddr_t code_start) {

  uint32_t cr3 = mmu_init_killer_task_dir();

  vaddr_t stack0 = mmu_next_free_kernel_page();
  vaddr_t esp0 = stack0 + PAGE_SIZE;
  return (tss_t){
      .cr3 = cr3,
      .esp = esp0,
      .ebp = esp0,
      .eip = (vaddr_t)code_startz, //como es identity maping dir_virt = dir_phy
      .cs = GDT_CODE_0_SEL,
      .ds = GDT_DATA_0_SEL,
      .es = GDT_DATA_0_SEL,
      .fs = GDT_DATA_0_SEL,
      .gs = GDT_DATA_0_SEL,
      .ss = GDT_DATA_0_SEL,
      .ss0 = GDT_DATA_0_SEL,
      .esp0 = esp0,
      .eflags = EFLAGS_IF,
  };
}
```

Nueva funcion en `mmu.c` para mapear paginas de codigo e identity maping del kernel, para ello vamos a utilizar el arreglo global que contiene el identity maping **kpd** en **mmu.c**

```C
paddr_t mmu_init_killer_task_dir() {

    //Nos hacemos un nuevo cr3
    paddr_t cr3 = mmu_next_free_kernel_page();


    //Una vez que tenemos el cr3 ahora inicializamo la pd
    pd_entry_t* pd = (pd_entry_t*)(CR3_TO_PAGE_DIR(cr3));

    //Con esto copiamos el identity maping
    pd[0] = kpd[0];

    //Como el codigo de la tarea vive dentro del identity maping al mapearlo ya es accesible

    return cr3;
}
```

Y por ultimo debemos modificar el `create_task` para que pueda crear la tarea killer, agregamos al arreglo en sched.c la direccion del inicio de la tarea (El cual asumo como dado en algun lugar del kernel), y por ultimo definidmo el nuevo tipo de tarea **TASK_KILLER**

```C
//Esto va en tasks.c pero lo pongo aca para no tener que
typedef enum {
  TASK_A = 0,
  TASK_B = 1,
  TASK_KILLER 2
} tipo_e;


static paddr_t task_code_start[2] = {
    [TASK_A] = TASK_A_CODE_START,
    [TASK_B] = TASK_B_CODE_START,
    [TASK_KILLER] = TASK_KILLER_CODE_START //Asumimos que lo tenemos definido
};
//Modificamos create task para que pueda crear la tarea killer
static int8_t create_task(tipo_e tipo) {
  size_t gdt_id;
  for (gdt_id = GDT_TSS_START; gdt_id < GDT_COUNT; gdt_id++) {
    if (gdt[gdt_id].p == 0) {
      break;
    }
  }
  kassert(gdt_id < GDT_COUNT, "No hay entradas disponibles en la GDT");
    // Modificacion de la funcion create task
    if (tipo == TASK_KILLER) {
        int8_t task_id = sched_add_task(gdt_id << 3);
        tss_task[task_id] = tss_create_killer_task(task_code_start[TASK_KILLER]);
        gdt[gdt_id] = tss_gdt_entry_for_task(&tss_tasks[task_id]);
        return task_id;
    }


  int8_t task_id = sched_add_task(gdt_id << 3);
  tss_tasks[task_id] = tss_create_user_task(task_code_start[tipo]);
  gdt[gdt_id] = tss_gdt_entry_for_task(&tss_tasks[task_id]);
  return task_id;
}
```

Lo ultimo que nos falta definir antes de la tarea de nivel 0, es una nueva estructuras para poder llevar la el conteo de ticks para las tareas **RUNNABLE**, para eso en sched.c definimos los siguiente:

**sched.c**

```C
//Cada indice corresponde a la id de la tarea y en cada posicion se lleva la cuenta de los ticks dedicados
uint8_t counter_ticks_per_task[MAX_TASK + 1];

//Definimos una funcion auxliar para aumentar los contadores

void inc_ticks_counter(void) {
    counter_ticks_per_task[current_task]++;
}
```

Por ultimo hay que modificar la isr_32 que se encargar del timer:

```ASM
extern inc_ticks_counter

global _isr32

_isr32:
  pushad
  call pic_finish1

  call inc_ticks_counter //aumentamos el contador de ticks de la tarea actual
  call sched_next_task

  str cx
  cmp ax, cx
  je .fin

  mov word [sched_task_selector], ax
  jmp far [sched_task_offset]

  .fin:
  call tasks_tick
  call tasks_screen_update
  popad
  iret
```

Por ultimo definimos la tarea killer, vamos a hacer lo de la misma manera en la que definiamos las tareas de usuario en el kernerl entonces tendriamos que nos queda asi:

```C



```
