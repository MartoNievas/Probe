import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Usuario {
    private final String nombre;
    private final String apellido;
    private final String email;
    private final int idSocio;
    private List<Libro> librosPrestados;

    //Constructor desde 0
    public Usuario(String nombre, String apellido, String email, int idSocio) {
        this.nombre = nombre;
        this.apellido = apellido;
        this.email = email;
        this.idSocio = idSocio;
        this.librosPrestados = new ArrayList<Libro>();
    }

    //constructor con libros prestados
    public Usuario(String nombre, String apellido, String email, int idSocio, List<Libro> librosPrestados) {
        this.nombre = nombre;
        this.apellido = apellido;
        this.email = email;
        this.idSocio = idSocio;
        this.librosPrestados = new ArrayList<Libro>(librosPrestados);
    }

    //geters
    public String getNombre() {return nombre;}

    public String getApellido() {return apellido;}

    public String getEmail() {return email;}

    public int getIdSocio() {return idSocio;}

    public List<Libro> getLibrosPrestados() {return Collections.unmodifiableList(librosPrestados);}

    //metodo para añadir libros

    public void addLibro(Libro libro) {
        this.librosPrestados.add(libro);
    }
    public void removeLibro(Libro libro) {
        if (!librosPrestados.contains(libro)) {
            return;
        }
        librosPrestados.remove(libro);
    }
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("{ \n" +
                "nomrbe: " + nombre + "\n" +
                "apellido: " + apellido + "\n" +
                "email: " + email + "\n" +
                "idSocio: " + idSocio + "\n" +
                "librosPrestados: " + librosPrestados.toString() + "\n" +
                "}");
    return sb.toString();
    }

}