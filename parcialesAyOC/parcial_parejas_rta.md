# Datos importantes: 
- Toda tarea pertenece como maximo a una pareja.
- Toda tarea puede abandonar su pareja.
- Las tareas pueden formar parejas cuantas veces quieran, pero solo pueden pertenecer a una en un momento dado.

- Las tareas de una pareja comparte un memoria de 4MB a partir de la direccion virtual `0xC0C00000`.
- Solo la tarea lider (que crea la pareja) puede escribir en la memoria compartida.
- Cuando una tarea abandona su pareja pierde acceso a los 4MB a partir de 0xC0C00000.


# Ejercicios: 

## Parte 1: implementación de las syscalls (80 pts)

1. (50 pts)

2. (20 pts)

3. (10 pts)

## Parte 2: monitoreo de la memoria de las parejas (20 pts)

4. 
