import java.util.Objects;

public class Tarea {
    private String nombre;
    private boolean completada;

    //Constructor
    public Tarea(String nombre, boolean completada) {
        this.completada = completada;
        this.nombre = nombre;
    }

    //Getters

    public String getNombre() {
        return nombre;
    }
    //Para verificar si esta completada
    public boolean isCompletada() {
        return completada;
    }

    //setters

    public void setNombre(String nombre) {
        this.nombre = nombre;
    }

    public void setCompletada(boolean completada) {
        this.completada = completada;
    }
    //Metodo equals y toString


    @Override
    public String toString() {
        return "Tarea{" +
                "nombre='" + nombre + '\'' +
                ", completada=" + ((completada) ? "si" : "no") +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        Tarea tarea = (Tarea) o;
        return completada == tarea.completada && Objects.equals(nombre, tarea.nombre);
    }
}
