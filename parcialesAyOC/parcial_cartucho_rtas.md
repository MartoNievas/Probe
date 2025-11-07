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
    - void buffer_dma(pd_entry_t* pd) que dado el page directory de una tarea realice el mapeo del buffer en modo DMA.
    - void buffer_copy(pd_entry_t* pd, paddr_t phys, vaddr_t virt) que dado el page directory de una tarea realice la copia del buffer a la dirección física pasada por parámetro y realice el mapeo a la dirección virtual pasada por parámetro.


# Ejercicio 2: 

- a) Programar la función void buffer_dma(pd_entry_t* pd)
- b) Programar la función void buffer_copy(pd_entry_t* pd, paddr_t phys, vaddr_t virt)


# Respuestas: 

1) 
a) Lo primero que debemos hacer es implementar la intrrupccion para el lector de cartuchos, lo que deberiamos a ser en grander rasgos es poner una nueva entrada en IDT para la rutina de antencion de la interrupccion del lector de cartuchos. 

## Estructuras a Modificar :

- **TASK_STATE**: Vamos a agregar un nuveo estado de tarea para poder representar el estado **BLOCKED**
- **IDT**: Tenemos que agregar una interrupcion por hardware y 2 syscalls.
- **SCHED_ENTRY** 

Quedaria de la siguiente manera: 

```C
typedef struc {
    uint16_t selector;
    task_state state;

    //Agregamos para resolver 
    //el mecanismo 
    uint32 copyDir; 
    uint8_t mode;
} sched_entry_t;

//En la idt no quedaria no siguiente:

void idt_init() {
    // ...
    IDT_ENTRY0(40);
    // ...
    IDT_ENTRY(90);
    IDT_ENTRY(91);
    // ...
}
```

En tasks.c modificamos lo siguiente: 

```C
typedef enum {
    DMA_MODE 0;
}
```

Vamos con deviceready, el cual estaria ubicado en idt.c. 

```C
void deviceready(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        sched_entry_t current_task = sched_tasks[i];
        if (current_task.state == BLOCKED) {
            if (current_task.mode == NO_ACCESS) continue;

            if(current_task.state == ACCESS_DMA) {
                buffer_dma(CR3_TO_PAGE_DIR(task_selector_to_cr3(current_task.selector)));
            }

            if (current_task.mode == ACCESS_COPY ) {
                buffer_copy(CR3_TO_PAGE_DIR(task_selector_to_cr3(current_task.selector)),
                mmu_next_user_page,current_task.mode);
            }
            current.state = TASK_RUNNABLE;
        } else {
            if (current_task.mode == ACCESS_COPY) {
                paddr_t dest = virt_to_phy(task_selector_to_cr3(current_task.selector),
                current_task.copyDir);
                //Esa direccion es la del buffer (siempre copya en el mismo);
                copy_page((paddr_t)0xF151C000,destino);
            }
        }
    }
}
```

Tambien debemos modificar la isr 

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
    if (sched_tasks[current_tasks].mode == ACCESS_COPY) {
        
    }
}
```