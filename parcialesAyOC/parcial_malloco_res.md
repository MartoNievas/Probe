# Ejercicio 1: (50 puntos)

Detallar todos los cambios que es necesario realizar sobre el kernel para que una tarea de nivel usuario pueda pedir memoria y liberarla, asumiendo como ya implementadas las funciones mencionadas. Para este ejercicio se puede asumir que la tarea 0 está implementada y funcionando.

## Respuesta:

Primero que nada `malloco` es una syscall, con lo cual va a poder ser llamada por tareas de usuario, pero en el fondo va a ser antendida por el kernel, comenzemos definiendo una nueve entrada en la **IDT**:

```C
IDT_ENTRY3(90)
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

Ahora debemos implementar el handler que se ocupa del manejo de la syscall que va a ser el siguiente:

```ASM
;malloco
global _isr90

_isr90:

pushad

push eax
call malloco ;La logica principal se la dejamos a molloaco que la vamos a implementar mas adelante
; Duda como hago para no borrar la direccion que se guarda en eax despues de malloco? DUDA!!!
add esp,4

popad
iret
```

Ahora como menciona el enunciado el sistema de **lazy allocation** entonces debemos solo mapear la pagina donde se encuentra la memoria en el momento donde se intenten acceder ya sea por una escritura o una lectura, esta accion va a arrojar un page_fault que va a ser atendida por la rutina `_isr14` la cual deriva su logica principal al **page_fault_handler** ya implementado en el tp, por lo que vamos a modificarlo para que se adapte al nuevo sistema:

```C
//Suponiendo que cada pagina es de 4kb tenemos 4MB por tarea
%define MAX_PAGES_BY_TASK 1024

//structs del enunciado
typedef struct {
  uint32_t task_id;
  reserva_t* array_reservas;
  uint32_t reservas_size;
} reservas_por_tarea;

typedef struct {
  uint32_t virt; //direccion virtual donde comienza el bloque reservado
  uint32_t tamanio; //tamaño del bloque en bytes
  uint8_t estado; //0 si la casilla está libre, 1 si la reserva está activa, 2 si la reserva está marcada para liberar, 3 si la reserva ya fue liberada
} reserva_t;

//Tenemos declardo como una variblabe global el arreglo:
// MAX_RESERVATION_ALLOWED no esta declarado porque en el enuncidado del ejercicio menciona que se puede asumir que tenemos el espacio suficiente
static reservas_por_tarea reservas_t[MAX_RESERVATION_ALLOWED] = {0};

//page fualt handler hecho en el tp
bool page_fault_handler(vaddr_t virt) {
  print("Atendiendo page fault...", 0, 0, C_FG_WHITE | C_BG_BLACK);
  // Chequeemos si el acceso fue dentro del area on-demand
  // En caso de que si, mapear la pagina

  if (virt >= ON_DEMAND_MEM_START_VIRTUAL &&
      virt <= ON_DEMAND_MEM_END_VIRTUAL) {
    uint32_t cr3 = rcr3();
    mmu_map_page(cr3, ON_DEMAND_MEM_START_VIRTUAL, ON_DEMAND_MEM_START_PHYSICAL,
                 MMU_P | MMU_W | MMU_U);
    tasks_screen_draw();
    return true;
  }

  //Verificamos que la memoria alla sido reservada por malloco, si no devuelvo false
  if (!esMemoriaReservada(virt)) return false;

  // Caso contrario lo es, entonces debemos mapear esto a paginas de usuario
  //Entiendo que no podemos asumir que la direccion que nos llega este alineada a 4kb entonces los hacemos

  vaddr_t ALINED_VADDR = virt & 0xFFFFF000 //Enmascaramos los 12 bits bajos de la direccion virtual.

  //Nos conseguimos el cr3 de la tarea actual
  uint32_t cr3 = rcr3();
  paddr_t PHY_PAGE = mmu_next_free_user_page(); //Preguntar si es memoria de usuario o a partir de una direccion especifica dentro de la misma
  zero_page(ALINED_VADDR); //Inicializamos

  mmu_map_page(cr3,ALINEAD_VADD,PHY_PAGE,MMU_P | MMU_W | MMU_U);

  return true;

}
```

Ahora vamos a utilizar la variable global declarada en `sched.c` la cual se llama **current_task** indica la tarea actual y vamos a declarar las dos siguiente funciones en `sched.c`.
Ademas vamos a modificar los estados que pueden tener las tareas para poder tener un estado **TASK_CLEAR**:

```C
uint8_t current_task = 0; //Asi es como se inicializa en sched.c
//Tambien hay un arreglo global el cual se llama:
typedef enum {
  TASK_SLOT_FREE,
  TASK_RUNNABLE,
  TASK_PAUSED,
  TASK_CLEAR //Nuevo estado para indicar que una tarea fue desalojada.
} task_state_t;

typedef struct {
  int16_t selector;
  task_state_t state;
} sched_entry_t;
//Para siempre poder tener el garbage collector corriendo.
static sched_entry_t sched_tasks[MAX_TASKS + 1] = {0};
//Desaloja la tarea actual
void sched_clear_current_task() {
  sched_tasks[current_task].state = TASK_CLEAR;
  //Deberia flushear la TSS o invalidar el descriptor? 
}
//Marcamos todas las reservas de la tarea acutal para liberar, de eso se va a encargar el garbaje collector
void sched_free_current_task() {
  for (int i = 0; i < MAX_RESERVATION_ALLOWED; i ++) {
    if (reservas_t[i].task_id == current_task) {
      for (int j = 0; j < reservas_t[i].reservas_size; j++) {
        reservas_t[i].array_reservas.estado = 2;
      }
    }
  }
}
```

Una vez platanteadas estas dos funciones auxiliares podemos utilizar el booleano que nos devuelve el handler de page fault para decidir si tenemos que continuar con la tarea actual o matarla y continuar con la siguiente. Con lo cual quedaria de la siguiente manera:

```ASM
_isr14:
    pushad
    mov esi, cr2        ; CR2 contiene la dirección virtual que causó el page fault
    push esi            ; pasamos la dirección como argumento
    call page_fault_handler
    add esp, 4          ; limpiamos el argumento
    cmp eax,0
    jne .ok

    call sched_clear_current_task ;Pausamos la tarea actual

    call sched_free_current_task ;Liberamos las reservas de la tarea actaul

    call sched_next_task ;llamamos para conmutar de tarea

    ;Aqui realizamos la conmutacion de tareas.
    str cx
    cmp ax, cx
    je .fin

    mov word [sched_task_selector], ax
    jmp far [sched_task_offset]

    .ok:
    popad
    add esp, 4          ; limpiamos el error code
    iret
```

# Ejercicio 2: (25 puntos)

Detallar todos los cambios que es necesario realizar sobre el kernel para incorporar la tarea garbage_collector si queremos que se ejecute una vez cada 100 ticks del reloj. Incluir una posible implementación del código de la tarea.

## Respuesta: 

Primero debemos crea un nuevo tipo de tarea en `tasks.c` la cual se va a llamar **GARBAGE_COLLECTOR**, luego nesecitamos crear la tss contamos con una funcion `tss_create_user_task`, podemos crear una parecida llamada `tss_create_system_task`, ademas vamos a usar la funcion `create_task`, para poder crearlar usando la funcion `tasks_init`. Vamos a detallar todos los cambios a continuacion: 

En tasks.c
```C
typedef enum {
  TASK_A = 0,
  TASK_B = 1,
  GARBAGE_COLLECTOR = 2; //Nuevo tipo de tarea.
} tipo_e;

static paddr_t task_code_start[3] = {
    [TASK_A] = TASK_A_CODE_START,
    [TASK_B] = TASK_B_CODE_START,
    [GARBAGE_COLLECTOR] = TASK_GARBAGE_COLLECTOR_START //Asumimos que existe porque no nos lo dan
};

//Modificacion de create_task

static int8_t create_task(tipo_e tipo) {
  size_t gdt_id;
  for (gdt_id = GDT_TSS_START; gdt_id < GDT_COUNT; gdt_id++) {
    if (gdt[gdt_id].p == 0) {
      break;
    }
  }
  kassert(gdt_id < GDT_COUNT, "No hay entradas disponibles en la GDT");

  //Agrego este if para ver si es la nueva tarea de nivel 0;
  if (tipo == GARBAGE_COLLECTOR) {
    int8_t task_id = sched_add_task(gdt_id << 3);
    tss_tasks[task_id] = tss_create_system_task(task_code_start[tipo]);
    gdt[gdt_id] = tss_gdt_entry_for_task(&tss_tasks[task_id]);
    return task_id;
  }
  int8_t task_id = sched_add_task(gdt_id << 3);
  tss_tasks[task_id] = tss_create_user_task(task_code_start[tipo]);
  gdt[gdt_id] = tss_gdt_entry_for_task(&tss_tasks[task_id]);
  return task_id;
}
```
Ahora en tss.c: 

```C
tss_t tss_create_system_task(paddr_t code_start) {

  uint32_t cr3 = mmu_init_task_system_dir(code_start);

  vaddr_t stack = mmu_next_free_kernel_page();

  vaddr_t esp0 = stack + (PAGE_SIZE-1);
  return (tss_t){
      .cr3 = cr3,
      .esp = stack,
      .ebp = stack,
      .eip = code_virt,
      .cs = GDT_CODE_0SEL,
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
Vamos a definir `mmu_init_task_system_dir` la cual le copia el identity maping del kernel la tarea

```C
paddr_t mmu_init_task_system_dir(paddr_t phy_start)
{
  // obtengo de mmu_next_free_kernel_page la direccion del nuevo page
  // directory
  // + lo inicializo en 0
  paddr_t task_pd_paddr = mmu_next_free_kernel_page();
  pd_entry_t *task_pd = (pd_entry_t *)task_pd_paddr;
  // defino en cr3 la base
  uint32_t cr3 = task_pd_paddr;
  // Copiar la entrada del kernel (kpd[0] para el identity mapping)
  // para que el kernel pueda operar cuando la tarea esté activa.
  task_pd[0] = kpd[0];

  //Ahora debemos mapear la pagina de codigo de la tarea (asumo que es una sola);
  mmu_map_page(cr3, TASK_GARBAGE_COLLECTOR_VITUAL, phy_start, MMU_P); //Tiene que ser de nivel kernel por eso solo el bit de presente
  
  return cr3;
}
```


# Para el garbage collector:

- 1. debo crear un nuevo tipo de tarea
- 2. debo hacer una funcion similar `tss_create_user_task` pero que se llame `tss_create_system_task`
- 3. Debo usar la funcion `create_task`, para crearla en la funcion tasks_init, debo crear un tarea de tipo GARBAGE_COLLECTOR, tambien podria aumentar el maximo de tareas que ahora es cuatro, para poder tener la 4 tareas de usuario y el garbage collector funcionando. Ademas deberia habilitarla con `sched_enable_task`.
- 4. Ahora con todo para inicializarla debo crear la tarea en si. El proceso que se va a estar corriendo:
  - Queremos que se ejecute cada 100 tiks de reloj entonces debemos tener un contador global de ticks, cuando llegue a 100 hacemos la limpieza, esta tiene que buscar las reservas y poder limpiarlas aquellas que tengan estado 2.
- 5.Modificar el scheduler
- 6. Tenemos que hacer la pd y pt para la tarea que van a estar en espacio de kernel. Funcion `mmu_init_system_task_dir` muy similar a `mmu_init_task_dir`, ¿al hacer esto tengo que mapearlo todo al kernel?

## Cosas que debo preguntar sobre garbage collector:

- 1. donde esta la direccion virtual de la tarea y fisica deberia ir la tarea. O puedo asumir en cualquier parte..
- 2. Cuantas paginas para codigo deberia asignar, no deberia inicializar una pila va a usar la del kernel.

# DUDAS QUE ME SURGEN:

AHORA ME SURGIO UNA DUDA, entiendo que el page_fault handler me va a retornar false si la posicion de memoria no es reservada por malloco osea es invalida y entonces salta una kernel_exception pero ahora deberia modificar la isr14 para cuando retorne false, desalojar la tarea, marcarla la memoria para que sea liberada y marcarla la tarea como pausada. Si la respuesta es si como puedo hacerlo sin conocer la id de la tarea.

`el kernel debe desalojar tarea inmediatamente y asegurarse de que no vuelva a correr, marcar la memoria reservada por la misma para que sea liberada y saltar a la próxima tarea.`

- para el ejercicio 1 puedo asumir que malloco esta hecho, si es asi para responder la interrucion tengo que usar la isr?
- que cosas debo tener en cuenta para poder pedir memoria dinamicamente.
- Tengo que cambiar el page_fault_handlre completamente o solo agregar esta region de memoria y dejar la on-demand
- Como se relacionan las estrcutrar que me dan y donde deberian ir.
- Para el ejercicio ademas de la entrada de la IDT, el isr90, que mas tengo que hacer.
