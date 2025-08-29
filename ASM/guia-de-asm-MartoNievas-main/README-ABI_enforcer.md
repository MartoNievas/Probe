# Guía de uso del ABI enforcer

Para los ejercicos de esta guía, que pueden encontrar en las subcarpetas dentro de `./src`, además de tests funcionales les proveemos una herramienta de validación de cumplimiento de ABI que se utilizará para evaluar sus entregas en las instancias de evaluación.
**No dejen de usar esta herramienta sobre sus soluciones de ASM!**

Vayan a leer los ejercicios y, antes de ponerse a programar, vuelvan acá a revisar las advertencias de uso del tester de ABI.

La herramienta consta de varios módulos que realizan distintos chequeos o instrumentan el programa de otros modos.
A continuación se comenta su funcionamiento y se hacen algunas advertencias a tener en cuenta al momento de autoevaluar sus soluciones.

> [!NOTE]
> Solicitamos que, de encontrar algún error o comportamiento inesperado en la herramienta, nos lo hagan saber por los canales de la materia. Gracias!

<h3>Tabla de contenidos</h3>

- [Guía de uso del ABI enforcer](#guía-de-uso-del-abi-enforcer)
  - [Recomendaciones de uso](#recomendaciones-de-uso)
  - [Modo de uso](#modo-de-uso)
  - [Módulos de la herramienta](#módulos-de-la-herramienta)
    - [:exclamation: Chequeo de offsets y tamaños de struct bien declarados](#exclamation-chequeo-de-offsets-y-tamaños-de-struct-bien-declarados)
    - [Chequeo de pila alineada al realizar `CALL`](#chequeo-de-pila-alineada-al-realizar-call)
    - [Polución de registros volátiles luego de realizar `CALL`](#polución-de-registros-volátiles-luego-de-realizar-call)
    - [:exclamation: \[PUEDE FALLAR\] Chequeo de preservación de registros no volátiles](#exclamation-puede-fallar-chequeo-de-preservación-de-registros-no-volátiles)
  - [Otros chequeos de ABI que no se evaluan en la herramienta:](#otros-chequeos-de-abi-que-no-se-evaluan-en-la-herramienta)
  - [Componentes de la herramienta](#componentes-de-la-herramienta)

## Recomendaciones de uso
Es común que bugs y errores de funcionamiento en las soluciones propuestas se deban a errores de ABI.
Esta herramienta debería ayudarlos a diagnosticar cuándo un error se debe a que no se esté cumpliendo alguna convención.
Recomendamos utilizar los tests lo más seguido posible durante la realización de los ejercicios, aunque si tienen dudas funcionales sobre su solución (si resuelve bien lo pedido, o maneja correctamente casos borde) y los chequeos de abi están introduciendo ruido extra, puede ser útil realizar alguna corrida sin los tests de ABI.

## Modo de uso
Se pueden correr los tests ejecutando el archivo `runAbi.sh` mediante el comando `./runAbi.sh`.
Pueden utilizar el comando `make clean` de antemano para limpiar los archivos temporales de compilaciones anteriores.

> [!NOTE]
> De fallar el comando `./runAbi.sh` con un mensaje como `./runAbi.sh: Permission denied`, ejecutar primero el comando `chmod u+x runAbi.sh` para darle permisos de ejecución al archivo.

Dicho comando realizará el `make abi_tester` correspondiente y luego ejecutará el binario resultante `./abi_tester` con **Valgrind**.

> [!WARNING]
> Se evaluará que las soluciones provistas en instancias de evaluación no den errores de memoria con la herramienta Valgrind.

Recomendamos fuertemente leer las próximas secciones que explican los aspectos de ABI evaluados y hacen advertencias sobre el funcionamiento de los módulos que componen la herramienta.

## Módulos de la herramienta

### :exclamation: Chequeo de offsets y tamaños de struct bien declarados
Objetivo: identificar el uso de offsets o tamaños de struct incorrectos

Este chequeo se apoya en las declaraciones provistas por el estudiante en los archivos `.asm` que incluyan definiciones con el siguiente formato (ejemplo):

```nasm
NODO_OFFSET_NEXT EQU ??
NODO_OFFSET_CATEGORIA EQU ??
NODO_OFFSET_ARREGLO EQU ??
NODO_SIZE EQU ??
```

Estas definiciones **no deben ser borradas** (de lo contrario los tests fallarán), solo se debe reemplazar los `??` por los números correspondientes a los offsets y tamaño de struct correspondientes.
En el archivo de definiciones de structs asociado (por ejemplo en esta guía `Estructura.asm` está asociado a `structs.h`) se declaran los nombres de las definiciones a buscar en ASM en asociación al campo de la misma línea. 

Por ejemplo, a partir de la declaración
```c
typedef struct nodo_s {
    struct nodo_s* next;   //asmdef_offset:NODO_OFFSET_NEXT       -> se va a revisar que el valor de la etiqueta NODO_OFFSET_NEXT coincida con el offset de este campo
    uint8_t categoria;     //asmdef_offset:NODO_OFFSET_CATEGORIA  -> se va a revisar que el valor de la etiqueta NODO_OFFSET_CATEGORIA coincida con el offset de este campo
    uint32_t* arreglo;     //asmdef_offset:NODO_OFFSET_ARREGLO    -> se va a revisar que el valor de la etiqueta NODO_OFFSET_ARREGLO coincida con el offset de este campo
    uint32_t longitud;     //asmdef_offset:NODO_OFFSET_LONGITUD   -> se va a revisar que el valor de la etiqueta NODO_OFFSET_LONGITUD coincida con el offset de este campo
} nodo_t; //asmdef_size:NODO_SIZE -> se va a revisar que el valor de la etiqueta NODO_SIZE coincida con el tamaño del struct nodo_t
```

### Chequeo de pila alineada al realizar `CALL`
Objetivo: evitar la realización de calls con la pila desalineada.

Se revisa que la pila se encuentre alineada (`rsp` tenga un valor módulo de 8 bytes) al momento de realizar todos los `call`. Si no se encuentra alineada, se imprime un mensaje de error y se detiene la ejecución.
Recuerden que la pila se debe alinear la pila a 16 bytes al realizar un call.

### Polución de registros volátiles luego de realizar `CALL`
Objetivo: evitar el uso de registros volátiles sin inicializar luego de un call.

Luego de cada call en el (o los) archivo .asm instrumentado, se inserta "basura" en los registros volátiles de acuerdo a ABI. Si les fallan tests al correr con el abi enforcer y no al correr sin, puede que estén usando registros volátiles como si fueran no volátiles en su implementación.

### :exclamation: [PUEDE FALLAR] Chequeo de preservación de registros no volátiles
Objetivo: revisar que las funciones programadas preserven los valores de registros no volátiles

Previo a cada `call` se guardan los valores de los registros no volátiles y luego se comparan con los valores de los mismos registros luego del `call`.
De no coincidir el valor de alguno de estos registros, se imprime una advertencia indicando el registro en infracción.

> [!WARNING]
> Por la naturaleza de la implementación, actualmente existe una (baja) posibilidad de que alguno de los registros modifique su valor entre el guardado de los valores y el llamado de la función, especialmente si la función a evaluar tiene muchos parámetros.
> Ante cualqueir advertencia de este módulo, recomendamos revisar en profundidad.
> De no encontrar problemas en su implementación, pueden desestimar la advertencia.
> Ante la duda, no dejen de consultar con los docentes.
> 
> En instancias de evaluación, los docentes revisarán manualmente casos donde salte la advertencia - **la aparición de estas advertencias puede no constituir un error en ciertas ocasiones**.

## Otros chequeos de ABI que no se evaluan en la herramienta:
En general, los siguientes errores resultan en errores funcionales siempre por lo que se espera que los estudiantes los resuelvan utilizando el debugger (gdb).
- Devolución de resultado por registro correcto (de hacerlo incorrectamente, los tests fallarán siempre (no se retornará el resultado esperado))
- Búsqueda de parámetros de función en registro correcto.
- Pasaje de parámetros de función por registros correctos.
- Uso de tamaños de registros correctos de acuerdo a los tipos de los parámetros pasados por parámetro.

Son bienvenidas y agradecidas contribuciones a la evaluación de estos aspectos o a la mejora de la herramienta existente!

