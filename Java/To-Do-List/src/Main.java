import java.util.Scanner;

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
public class Main {
    public static void main(String[] args) {
        int opcion;
        Scanner scanner = new Scanner(System.in);
        ToDoListApp lista = new ToDoListApp();

        do {
            System.out.println("\n====== TO-DO LIST ======");
            System.out.println("1. Agregar tarea");
            System.out.println("2. Listar tareas");
            System.out.println("3. Marcar tarea como completada");
            System.out.println("4. Eliminar tarea");
            System.out.println("5. Para consultar el promedio de tareas completadas");
            System.out.println("6. Salir");
            System.out.println("Opcion: ");
            opcion = scanner.nextInt();
            scanner.nextLine();
            switch (opcion) {
                case 1 -> {
                    System.out.println("Ingrese la descripcion de la tarea: ");
                    var descricion = scanner.nextLine();
                    try {
                        lista.agregarTarea(descricion);
                        System.out.println("La tarea se agreggo con exito.");
                    } catch (IllegalArgumentException e) {
                        e.getCause();
                        e.getMessage();
                        System.out.println("Fallo al agregar la tarea a la lista");
                    }
                }

                case 2 -> {
                    lista.listarTareas();
                }
                case 3 -> {
                    System.out.println("Ingrese el nombre de la tarea a marcar como completada: ");
                    lista.completada(scanner.nextLine());
                }
                case 4 -> {
                    System.out.println("Ingrese el nombre d ela tarea a eliminar: ");
                    var nombre = scanner.nextLine();

                    if (lista.eliminarTarea(nombre)) {
                        System.out.println("Tarea eliminada correctamente");
                    } else {
                        System.out.println("No se pudo encontrar la tarea");
                    }
                }
                case 5 -> {
                    System.out.println("El promedio de tareas completadas es de: " + lista.promedioTareasCompletadas());
                }
                case 6 -> {
                    System.out.println("Saliendo del programa...");
                }
            }
        } while (opcion != 6);
        scanner.close();
    }
}