# Primer parcial - Arquitectura y Organización del Computador - Comision David
### Segundo cuatrimestre 2025
- [Normas generales y modalidad de entrega](#normas-generales-y-modalidad-de-entrega)
	- [Régimen de Aprobación](#régimen-de-aprobación)
	- [Conocimientos a evaluar](#conocimientos-a-evaluar)
	- [Compilación y Testeo](#compilación-y-testeo)
	- [Verificación de tests](#verificación-de-tests)
- [Enunciado](#enunciado)
- [Estructuras](#estructuras)
	- [Ejercicio 1](#ejercicio-1)
	- [Ejercicio 2](#ejercicio-2)
	- [Ejercicio 3](#ejercicio-3)



# Normas generales y modalidad de entrega

- El parcial es **INDIVIDUAL**.
- Una vez terminada la evaluación se deberá crear un PR al `main` de su repo desde la branch con la resolución como source.

> [!CAUTION]
> Es importante que no modifiquen los archivos de los tester. Si lo hacen, se nos advertirá y tendremos que desaprobar la entrega automáticamente.
> 
> **No se deben modificar manualmente:**
> - las carpetas test_utils y .github
> - los Makefiles
> - los test.c (salvo para comentar algún test y después dejarlo como estaba originalmente)

## Régimen de Aprobación

- El parcial es en los laboratorios, usando las compus de los labos o sus propias compus.
- Es a libro abierto, pueden tener todo lo que se les ocurra a disposición. Está totalmente prohibido el uso del celular, internet y chatbots.
- Deberán conectarse a un proxy que limitará su acceso a internet para que únicamente se puedan conectar a GitHub. 
- Les proveeremos de un script que deberá ejecutarse al comienzo del parcial y no detenerse hasta haber entregado. Con él nos aseguramos que permanezcan conectados al proxy.
- **Sólo se evalúa programación en Assembly**, **C no se corrige** pero recomendamos que primero planteen una solución en C y después pasarla a Assembly cuando tengan en claro cómo estructurar la solución.
- Las funciones auxiliares que implementen también deben estar en Assembly.
- Para aprobar el parcial deben implementar 2 de los 3 puntos del enunciado y que corran con éxito los tests funcionales, de abi y de memoria (valgrind).
- Vamos a usar herramientas de detección de plagio para asegurarnos de que su entrega sea original.

> [!NOTE]
> El comando `./tester.sh` corre los mismos tests que el juez online. Pueden usarlo para revisar que su parcial vaya a pasar los tests en dicho entorno.

> [!NOTE]
> Durante el parcial estaremos disponibles para resolver consultas de enunciado y para destrabarles si están dando vueltas mucho tiempo en algo que no forma parte de lo evaluado. NO responderemos preguntas teóricas.


## Conocimientos a evaluar 
- Uso de memoria dinámica.
- Navegación de punteros.
- Representación binaria de los tipos de datos.
- Manejo de pila.
- Manejo de structs.
- Convención C, ABI, uso de registros.
- Uso del debugger GDB.

## Compilación y Testeo

Para compilar y ejecutar los tests en cada ejercicio se dispone de un archivo
`Makefile`, con los siguientes *targets*:

| Comando             | Descripción                                                         |
| ------------------- | ------------------------------------------------------------------- |
| `make test_c`       | Genera el ejecutable usando la implementación en C del ejercicio.   |
| `make test_asm`     | Genera el ejecutable usando la implementación en ASM del ejercicio. |
| `make test_abi`     | Genera el ejecutable usando la implementación en ASM del ejercicio + archivos necesarios para ABI enforcer. |
| `make run_c`        | Corre los tests usando la implementación en C.                      |
| `make run_asm`      | Corre los tests usando la implementación en ASM.                    |
| `make run_abi`      | Corre los tests usando la implementación en ASM + ABI enforcer.     |
| `make valgrind_c`   | Corre los tests en valgrind usando la implementación en C.          |
| `make valgrind_asm` | Corre los tests en valgrind usando la implementación en ASM.        |
| `make valgrind_abi` | Corre los tests en valgrind usando la implementación en ASM + ABI enforcer.        |
| `make check_offsets` | Corre el chequeo de offsets.                                         |
| `make run_main_c`        | Corre main usando la implementación en C.                      |
| `make run_main_asm`      | Corre main usando la implementación en ASM (no corre ABI enforcer).                    |
| `make clean`        | Borra todo archivo generado por el `Makefile`.                      |


## Verificación de tests
Para el parcial, contamos con una máquina de la facultad para correr los tests en un entorno limpio y controlado. La idea es que ustedes trabajen en su branch, haciendo los commits y push que necesiten, hasta tener los tests pasando en su computadora local.

> [!CAUTION]
> Para considerar un ejercicio aprobado, debe pasar los tests **con el comando** `make valgrind_abi`.

Una vez que tengan el parcial para entregar, procedan a revisar su aprobación del siguiente modo:
- Crean un PR de `su branch` a `main` con los cambios a entregar.
- **PARA CORRER LOS TESTS, DEBEN AGREGAR la ETIQUETA (label) "tests" AL PR**, verán en su repositorio que aparece una label `tests` a tal fin.
- Cuando se detecte que agregaron la label, denle uno o dos minutos (puede tardar más si hay muchas personas entregando al mismo tiempo) y se comenzarán a correr los tests. El estado de los mismos se informará en la ventana del PR donde figura el botón para hacer el merge.
- Eventualmente terminarán los tests y dirá si pasaron (aprobado) o no. Pueden revisar el progreso si hacen click en el nombre de la corrida en curso.
- **No hacer el merge a main del PR**.

> [!NOTE]
> Si hacen cambios y quieren volver a correr los tests, deben SACAR LA ETIQUETA, guardar el cambio (pueden refrescar la pagina por ejemplo) Y VOLVER A AGREGAR LA ETIQUETA.

# Enunciado

Un prestigioso marketplace de latinoamérica, el famoso Orga2-Libre, anda necesitando ayuda para poder mejorar su motor de búsqueda. Para eso, nos contactaron a nosotros para poder ayudarlo en esta tarea.


Dentro del search existen items
## Estructura: Producto

```C
typedef struct {
	usuario_t* usuario;
	char categoria[9];
	char nombre[25];
	uint16_t estado;
	uint32_t precio;
	uint32_t id;
} producto_t;
```

Esta estructura representa un Producto. Contiene:

- `usuario`: puntero al usuario que publicó el ítem.
- `categoria`: Categoría del producto.
- `nombre`: Nombre del producto.
- `estado`: puede tomar los valores:
  - `0`: usado
  - `1`: nuevo

## Estructura: Usuario

```C
typedef struct {
	uint32_t id;
	uint8_t nivel;
} usuario_t;
```

Donde:

- `id`: identificador único del usuario.
- `nivel`: puede ser `0`, `1` o `2`, representando el nivel del cliente dentro de la plataforma. Si el nivel es 1 o superior, se considera que el usuario está verificado.

---

## Ejercicio 1

Para asegurar la calidad de las publicaciones visibles, nos solicitan implementar un filtro que seleccione los productos nuevos (Aquellos que tengan 1 como valor de estado) que pertenezcan a usuarios verificados (nivel 1 o superior). Contamos con el catálogo de productos almacenado en una lista enlazada definida del siguiente modo:

```C
typedef struct {
	struct publicacion_t *next;
	producto_t *value;

} publicacion_t;
```
```C
typedef struct {
	struct publicacion_t *first;
} catalogo_t;
```

Así que vamos a implementar una funcion en asm que reciba un puntero al catálogo y retorne un arreglo con referencias a todos los productos que cumplan con las condiciones.

```C
producto_t** filtrarPublicacionesNuevasDeUsuariosVerificados (catalogo_t*)
```

> [!CAUTION]
> El arreglo devuelto debe tener un último elemento nulo para indicar el final.

Consejo: Pueden hacer una función auxiliar que determine si un producto dado cumple las condiciones.

---

## Ejercicio 2

En esta plataforma, tener un producto publicado no tiene costo para los vendedores, esto hace que algunos usuarios se abusen y publiquen varias veces el mismo producto para tener más visibilidad. Queremos poder detectar estos casos para poder quitarlos.

Para eso vamos a implementar una funcion en ASM 
```C
catalogo_t* removerCopias(catalogo* catalogo_t)
```

que toma un puntero a un catálogo y devuelve la misma lista pero dejando **el primer ejemplar** de cada publicación. Una publicación se considera repetida si ya existe **otra** publicación con el mismo producto. Consideramos a dos productos como el mismo si el nombre y el usuario de ambos son iguales. La memoria de las publicaciones y los productos removidos debe ser liberada, no así la memoria del usuario referenciado en el mismo.

Recomendación: Hacer una funcion auxiliar `removerAparicionesPosterioresDe(publicacion_t* publicacion)`. Recibe una publicación dentro de un catálogo y remueve las publicaciones posteriores que sean iguales a esta. 

> [!CAUTION]
> Ojo que al liberar al producto no hay que liberar al usuario. 

> [!NOTE]
> Se puede confiar en que no habrá dos **instancias** iguales de un usuario dado.

---

## Ejercicio 3

En el ecosistema de Orga2-Libre hay distintas aplicaciones que contienen información variada sobre aquellos usuarios registrados en la plataforma. La parte encargada de compras (Aquella con la que estuvimos trabajando hasta ahora) tiene su propio registro de usuarios pero interactúa con un sistema externo para corroborar el nivel de verificación de los usuarios.

Necesitamos implementar la función
```C
usuario_t** asignarNivelesParaNuevosUsuarios(uint32_t* ids, uint32_t cantidadDeIds, uint8_t (*deQueNivelEs)(uint32_t))
```
que toma un arreglo de `ids` con su tamaño y una función que determina el nivel de un usuario, y devuelve un arreglo con nuevos usuarios. Si el arreglo de ids es vacío, se debe retornar NULL.
