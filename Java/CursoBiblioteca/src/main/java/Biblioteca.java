import java.util.ArrayList;
import java.util.List;

public class Biblioteca {
    private List<Usuario> usuarioList;
    private List<Libro> libroList;

    //Contructor vacio
    public Biblioteca(List<Libro> libros, List<Usuario> users) {
        this.usuarioList = users;
        this.libroList = libros;
    }


    //metodos
    public boolean agregarLibro(Libro libro) {
        for (Libro l : libroList) {
            if (l.getTitulo().equalsIgnoreCase(libro.getTitulo())
                    && l.getAutor().equalsIgnoreCase(libro.getAutor())) {
                System.out.println("Error: El libro " + libro.getTitulo() + " de " + libro.getAutor() + " ya existe en el catalogo.");
                return false;
            }

        }
        libro.setDisponible(true);
        libroList.add(libro);
        System.out.println("El libro: " + libro.getTitulo() + " fue agregado con exito.");
        return true;
    }

    public boolean eliminarLibro(Libro libro) {
        Libro libroAEliminar = buscarLibroPorTituloYAutor(libro.getTitulo(), libro.getAutor());
        if (libroAEliminar == null || !libroAEliminar.isDisponible()) {
            System.out.println("El libro: " + libro.getTitulo() + " no existe.");
            return false;
        }
        libroList.remove(libroAEliminar);
        System.out.println("El libro: " + libroAEliminar.getTitulo() + " fue eliminado con exito.");
        return true;
    }

    public Libro buscarLibroPorTituloYAutor(String titulo, String autor) {
        for (Libro libro : libroList) {
            if (libro.getTitulo().equalsIgnoreCase(titulo) &&
                    libro.getAutor().equalsIgnoreCase(autor)) {
                return libro;
            }
        }
        return null;
    }

    public void listarLibrosDisponibles() {
        System.out.println("\n--- Libros Disponibles ---");
        boolean encontrado = false;
        for (Libro libro : libroList) {
            if (libro.isDisponible()) {
                System.out.println(libro.toString());
                encontrado = true;
            }
        }
        if (!encontrado) {
            System.out.println("No hay libros disponibles en el catalogo en este momento.");
        }
    }

    public void listarTodosLosLibros() {
        System.out.println("\n --- Catalogo completo ---");
        if (libroList.isEmpty()) {
            System.out.println("No hay libros disponibles en el catalogo.");
            return;
        }
        for (Libro libro : libroList) {
            System.out.println(libro.toString());
        }
    }

    public boolean registrarUsuario(Usuario user) {
        for (Usuario u : usuarioList) {
            if (u.getIdSocio() == user.getIdSocio()) {
                System.out.println("Error: El usuario con ID " + user.getIdSocio() + " ya se encuentra registrado.");
                return false;
            }
        }
        usuarioList.add(user);
        System.out.println("El usuario " + user.getIdSocio() + " fue registrado con exito.");
        return true;
    }

    public boolean eliminarUsuario(Usuario user) {
        Usuario usuarioAEliminar = buscarUsuarioPorID(user.getIdSocio());
        if (usuarioAEliminar != null) {
            if (!usuarioAEliminar.getLibrosPrestados().isEmpty()) {
                System.out.println("El usurario: " + usuarioAEliminar.getIdSocio() + " no se puede eliminar porque tiene libros prestdos.");
                return false;
            }
            usuarioList.remove(usuarioAEliminar);
            System.out.println("El usuario: " + usuarioAEliminar.getIdSocio() + " fue eliminado con exito.");
            return true;
        }
        System.out.println("El usuario: " + user.getIdSocio() + " no esta registrado.");
        return false;
    }

    public Usuario buscarUsuarioPorID(int ID) {
        for (Usuario user : usuarioList) {
            if (user.getIdSocio() == ID) {
                return user;
            }
        }
        return null;
    }

    public void listarUsuariosRegistrados() {
        System.out.println("\n --- Lista de Usuarios Registrados ---");
        for (Usuario user : usuarioList) {
            System.out.println(user.toString());
        }
    }

    public boolean prestarLibro(String titulo, String autor, int IdUsuario) {
        Libro libro = buscarLibroPorTituloYAutor(titulo, autor);
        Usuario user = buscarUsuarioPorID(IdUsuario);
        if (libro == null) {
            System.out.println("El libro: " + titulo + " del autor: " + autor + " no existe en el catalogo.");
            return false;
        }
        if (user == null) {
            System.out.println("El usurario: " + IdUsuario + " no esta registrado.");
            return false;
        }
        if (!libro.isDisponible()) {
            System.out.println("El libro: " + titulo + " del autor: " + autor + " no esta disponible");
            return false;
        }
        if (user.getLibrosPrestados().contains(libro)) {
            System.out.println("El libro: " + libro.getTitulo() + " del autor: " + libro.getAutor() + " ya lo tiene el usuario.");
            return false;
        }
        libro.setDisponible(false);
        user.addLibro(libro);
        System.out.println("El libro: " + titulo + " fue prestado a: " + IdUsuario + ".");
        return true;

    }

    public boolean devolverLibro(String titulo, String autor, int id) {
        Libro libro = buscarLibroPorTituloYAutor(titulo, autor);
        Usuario user = buscarUsuarioPorID(id);
        if (libro == null) {
            System.out.println("El libro: " + titulo + " del autor: " + autor + " no existe en el catalogo.");
            return false;
        }
        if (user == null) {
            System.out.println("El usurario: " + id + " no esta registrado.");
            return false;
        }
        if (libro.isDisponible()) {
            System.out.println("Error del sistema el libro que se quiere devolver ya se encuentra disponible.");
            return false;
        }
        if (!user.getLibrosPrestados().contains(libro)) {
            System.out.println("El libro: " + libro.getTitulo() + " del autor: " + libro.getAutor() + " no lo tiene el usuario.");
            return false;
        }
        libro.setDisponible(true);
        user.removeLibro(libro);
        System.out.println("Se devolvio el libro: " + libro.getTitulo() + " por el usuario: " + user.getIdSocio() + " con exito.");
        return true;
    }

    public void listarLibrosPrestadosDelUsuario(int id) {
        Usuario user = buscarUsuarioPorID(id);
        if (user == null) {
            System.out.println("El usuario de id: " + id + " no existe.");
            return;
        }
        System.out.println("\n --- Libros prestados al usuario: " + user.getNombre() + " ---");
        List<Libro> librosDelUsuario = user.getLibrosPrestados();
        if (librosDelUsuario.isEmpty()) {
            System.out.println("El usurario: " + user.getNombre() + " no tiene libros prestados.");
            return;
        }
        for (Libro libro : librosDelUsuario) {
            System.out.println(libro.toString());
        }
    }

    public List<Libro> getLibroList() {
        return new ArrayList<>(libroList);
    }

    public List<Usuario> getUsuarioList() {
        return new ArrayList<>(usuarioList);
    }
}