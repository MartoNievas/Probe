# Ejercicio 1: (50 puntos)

Detallar todos los cambios que es necesario realizar sobre el kernel para que una tarea de nivel usuario pueda pedir memoria y liberarla, asumiendo como ya implementadas las funciones mencionadas. Para este ejercicio se puede asumir que la tarea 0 está implementada y funcionando.

## Respuesta:

Primero que nada `malloco` es una syscall, con lo cual va a poder ser llamada por tareas de usuario, pero en el fondo va a ser antendida por el kernel, comenzemos definiendo una nueve entrada en la **IDT**:

```C
IDT_ENTRY3(90) //maloco
IDT_ENTRY(91) //chau
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
En ambos casos asumimos que se pasa como parametro por eax.

```ASM
mov eax, <Cantidad de bytes a allocar>
int 90

mov eax, <Cantidad de bytes a allocar>
int 91
```

Ahora debemos implementar el handler que se ocupa del manejo de la syscall que va a ser el siguiente:

```ASM
;malloco
global _isr90

_isr90:

pushad

push eax
call malloco ;La logica principal se la dejamos a molloaco que la vamos a implementar mas adelante
;Sabemos que malloco devuelve una direccion virtual por eax pero si hacemos popad perdemos esa direccion
;Lo que vamos a hacer es guardarlo en la pila antes del popad
add esp,4
mov dword [esp + 28], eax ;Porque asi es el orden en el que se pushean los registros en la pila, sacado del manual de intel
;Con eso nos aseguramos de devolver en eax la direccion virtual del espacio reservado
popad
iret

global isr_91

isr_91:

  pushad

  push eax
  call chau
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

static reservas_por_tarea reservas[MAX_TASKS + 1] = {0};

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
  paddr_t cr3 = rcr3();
  paddr_t PHY_PAGE = mmu_next_free_user_page(); //Preguntar si es memoria de usuario o a partir de una direccion especifica dentro de la misma
  zero_page(ALINED_VADDR); //Inicializamos

  mmu_map_page(cr3,ALINEAD_VADD,PHY_PAGE, MMU_P | MMU_W | MMU_U);

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
  reservas_por_tarea reserva = reservas[current_task];
  for (int j = 0; j < reserva.reservas_size; j++) {
      reserva.array_reservas.estado = 2;
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
    [GARBAGE_COLLECTOR] = TASK_GARBAGE_COLLECTOR_START //El codigo de inicio de la tarea esta dado por esta etqiueta
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

Ahora en tss.c, modificamos la creaccion de la tss para que solo tenga stack de nivel 0, use los segmentos de codigo y datos de nivel 0:

```C
tss_t tss_create_system_task(paddr_t code_start) {

  uint32_t cr3 = mmu_init_task_system_dir();
  vaddr_t code_virt = (vaddr_t)code_start; //Direccion virtual de la tarea. es la mimsma que la fisica
  vaddr_t stack = mmu_next_free_kernel_page();

  vaddr_t esp0 = stack + (PAGE_SIZE - 1);
  return (tss_t){
      .cr3 = cr3,
      .esp = esp0,
      .ebp = esp0,
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

Vamos a definir `mmu_init_task_system_dir` la cual le copia el identity maping del kernel a la tarea y ademas mapea la pagina de codigo en este ejercicio asumo que solo es una pagina de codigo, Ademas asumo que la pila ya esta mapeada, por ultimo para copiar el identity maping usamos la varibale global `kpd` la cual es el **page directory** del kernel que se encuenta en `mmu.c`.

```C
paddr_t mmu_init_task_system_dir()
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

  //Como el codigo de la tarea vive en el kernel ya esta mapeada por el identity maping, y el comienzo del codigo de la tarea esta dado por la etiqueta TASK_GARBAGE_COLLECTOR_START.

  return cr3;
}
```

Tambien deberiamos cambiar `tasks_init` en `tasks.c` y quedaria de la siguiente manera: 

```C
void tasks_init(void) {
  int8_t task_id;
  // Dibujamos la interfaz principal
  tasks_screen_draw();

  // Creamos las tareas de tipo A
  task_id = create_task(TASK_A);
  sched_enable_task(task_id);
  task_id = create_task(TASK_A);
  sched_enable_task(task_id);
  task_id = create_task(TASK_A);
  sched_enable_task(task_id);

  // Creamos las tareas de tipo B
  task_id = create_task(TASK_B);
  sched_enable_task(task_id);


  //Agregamos esta lines para habilitar la nueva tarea. 
  task_id = create_task(GARBAGE_COLLECTOR);
  sched_enable_task(task_id); 
}
```

Una vez que tenenmos todas las estructuras modificadas y adaptadas a la nueva tarea de nivel 0, podemos pensar en la implementacion de la misma, aqui vamos a asumir que malloco siempre tiene direcciones alineadas para poder simplificar la limpieza de las paginas, ya que vamos a utilizar la funcion `mmu_unmap_page` para poder mapear las paginas y asi quedaran libres para ser utilizadas en el futuro.
Implementacion de la tarea es igual que el resto de las tareas del sistema, es una tarea perpetua.

```C
void task(void) {

  while(true) {
    for (int i = 0; i < MAX_TASKS + 1; i ++) {
      reservas_por_tarea* reserva = dameReservas(i);
      for (int j = 0; j < reserva->reservas_size; j ++) {
        if (reserva->array_reservas[j]->estado == 2) {
          tss* tss_target = &tss_tasks[reserva.task_id]; //Buscamos el tss para poder obtener el cr3 actual y desmapear la pagina
          
          

          paddr_t cr3 = tss_target->cr3;
          //Asumiendo que esta direccion esta alineada a 4kb si no deberiamos enmascararlo.
          vaddr_t virt = reserva.array_reservas[j].virt;
          
          //Indicamos que la liberamos
          reserva->estado = 3;

          mmu_unmap_page(cr3,virt);
        }
      }
    }
  }
}
```

Por ultimo debemos modificar el scheduler para que cada 100 ticks llame a esta tarea de nivel 0, entonces debemos tener una cuanta de los ticks globales para eso vamos a declarar una variable global en `sched.c` llamada **ticks_counter** un uint8_t y una funcion tambien en `sched.c` la cual se va a encargar de aumentar los ticks en la interrupcion del timer.

El codigo en sched.c nos quedaria asi, ademas voy a asumir que la entrada de la **GDT** donde se encuentra la tarea **GARBAGE_COLLECTOR** es la 20:

```C
uint8_t ticks_counter = 0;

void inc_ticks_counter(void) {
  ticks_counter ++;
}

uint16_t sched_next_task(void) {
  // Buscamos la próxima tarea viva (comenzando en la actual)
  if (ticks_counter == 100) {
    ticks_counter = 0;
    return (20 << 3); // Este va a ser el tr que indica la entrada de la gdt que describe la tss de la tarea.
  }

  int8_t i;
  for (i = (current_task + 1); (i % MAX_TASKS) != current_task; i++) {
    // Si esta tarea está disponible la ejecutamos
    if (sched_tasks[i % MAX_TASKS].state == TASK_RUNNABLE) {
      break;
    }
  }

  // Ajustamos i para que esté entre 0 y MAX_TASKS-1
  i = i % MAX_TASKS;

  // Si la tarea que encontramos es ejecutable entonces vamos a correrla.
  if (sched_tasks[i].state == TASK_RUNNABLE) {
    current_task = i;
    return sched_tasks[i].selector;
  }

  // En el peor de los casos no hay ninguna tarea viva. Usemos la idle como
  // selector.
  return GDT_IDX_TASK_IDLE << 3;
}
```

Y tembien vamos a modificar la interrupcion del timer quedaria de la sigueinte manera:

```ASM
extern inc_ticks_counter
global _isr32

_isr32:
  pushad
  call pic_finish1
  call inc_ticks_counter //Incrementamos el contador de ticks
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

# Ejercicio 3:

a)Indicar dónde podría estár definida la estructura que lleva registro de las reservas (5 puntos).

b)Dar una implementación para malloco (10 puntos).

c)Dar una implementación para chau (10 puntos).

## Respuesta:

a) La estructura que se lleva el registro de las reservas podria estar definido en el modulo de la `mmu.c` al igual que los wrappers de las systems calls que serian malloco y chau. Esto se debe a la realacion que hay entre este nuevo mecanismos que queremos implementar y lo ya existente con respecto al TP.

b) Vamos con la implmentacion de malloco teniendo en cuenta la considreacion mencionadas en el enunciado, para ello vamos a tener en cuenta que reservas_size es la cantidad de reservas activas

```C
//Tenemos los siguientes defines
#define MAX_RESERVA_TAREA (4 * 1024 * 1024) // cantidad maxima de memoria que se puede reservar
#define BASE_RESERVABLE 0xA10C0000 //la direccion base donde se empieza a reservar

void* malloco(size_t size) {

  uint32_t task_id = current_task;
  reservas_por_tarea* registro = dameReservas(task_id); //tenemos el puntero a la reserva de la tarea actual
  reserva_t* reserva = registro->array_reservas[registro->reservas_size-1];

  vaddr_t virt = reserva.virt;
  uint32_t memoria_total = virt + reserva.tamanio;
  
  if (memoria_total - BASE_RESERVABLE + size >= MAX_RESERVA_TAREA) return NULL;
  
  //Esto significa que no hay espacio para la tarea.
 
  //Si todavia queda espacio
  vaddr_t dir_incio = ultima_dir;

  //tenemos que crear una nueva reserva
  reserva_t* nueva = &registro->array_reservas[registro->reservas_size];
  registro->reservas_size++;
  nueva->virt = dir_inicio;
  nueva->estado = 1; //Esta activa
  nueva-> tamanio = size;

  return (void*)dir_inicio;
}
```

c) Para implementar chau vamos a hacer de la siguiente manera: buscamos en las reservas de la tarea actual, si no esta no hacemos nada, si esta verificamos que tanga estado 1 y la marcamos como estado 2 (para ser liberada por el GARBAGE_COLLECTOR), si tiene estado 0 o 2 no hago nada, y una vez que la encuentro y modifico termino.

```C
void chau(vaddr_t virt) {
  if (!esMemoriaReservada(virt)) return;

  reserva_por_tarea* registro = dameReservas(current_task);
  for (int i = 0; i < registro->reserva_size; i++) {
    reserva_t* r = &registro->array_reservas[i];
    //Todavia no fue liberado
    if ((r->estado == 1 || r->estado == 0) && r->virt == virt) {
      r-> estado = 2;
      return;
    }
    //Ya fue liberado
    if ((r->estado == 3 || r->estado == 2) && r->virt == virt) {
      return;
    }
  }


}
```

## DUDAS QUE ME QUEDARON:
