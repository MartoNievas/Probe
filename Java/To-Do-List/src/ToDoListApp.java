import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.List;
import java.util.function.Function;

public class ToDoListApp {
    private List<Tarea> listaTareas;
    int cantidadDeTareas;
    int cantidadDeTareasCompletadas;
    //Constructor
    public ToDoListApp() {
        listaTareas = new ArrayList<>();
    }

    public void agregarTarea(String nombre)  throws IllegalArgumentException{
        Tarea tarea = new Tarea(nombre,false);
        listaTareas.forEach((tarea1 -> {
            if (tarea.equals(tarea1)) throw new IllegalArgumentException("La tarea ya forma parte de la lista");
        }));
        listaTareas.add(tarea);
        cantidadDeTareas ++;

    }

    public void listarTareas() {
        if(listaTareas.isEmpty()) {
            System.out.println("La lista de tareas esta vacia.");
            return;
        }
        System.out.println("---PENDIESES---");
        listaTareas.stream().
                filter(t -> !t.isCompletada()).
                forEach((tarea) -> System.out.println(tarea.toString()));
        System.out.println("---COMPLETADAS");
        listaTareas.stream().
                filter(Tarea::isCompletada).
                forEach(t -> System.out.println(t.toString()));
    }

    public void completada(String tarea) {
        boolean encontrada = false;

        for (Tarea t : listaTareas) {
            if (t.getNombre().equalsIgnoreCase(tarea)) {
                t.setCompletada(true);
                System.out.println(t + " marcada como completada");
                encontrada = true;
                cantidadDeTareasCompletadas ++;
                continue;
            }
            if (!encontrada) {
                System.out.println("No se encontro la tarea.");
            }

        }
    }
    public boolean eliminarTarea(String nombre) {
        return listaTareas.removeIf((tarea -> tarea.getNombre().equalsIgnoreCase(nombre)));
    }
    public float promedioTareasCompletadas() {
        if(cantidadDeTareas == 0) {
            System.out.println("Todavia no hay tareas asignadas");
            return 0;
        }
        return (float) cantidadDeTareasCompletadas / cantidadDeTareas;
    }
}
