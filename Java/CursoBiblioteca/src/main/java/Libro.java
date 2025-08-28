
public class Libro {
    private final String titulo;
    private final String autor;
    private final String genero;
    private final int añoPublicacion;
    private boolean disponible;
    //Constructor a partir de un builder
    public Libro(Builder builder) {
        this.autor = builder.autor;
        this.titulo = builder.titulo;
        this.genero = builder.genero;
        this.añoPublicacion = builder.fechaPublicacion;
        this.disponible = builder.disponilbe;

    }
    //Ahora configuramos los getters
    public String getTitulo() {
        return titulo;
    }
    public String getAutor() {
        return autor;
    }
    public String getGenero() {
        return genero;
    }
    public int getAñoPublicacion() {
        return añoPublicacion;
    }
    public boolean isDisponible() {
        return disponible;
    }

    //Para setear la disponibilidad ya que es mutable
    public void setDisponible(boolean disponible) {
        this.disponible = disponible;
    }
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("{ \n" +
                "Titulo: " + titulo + "\n" +
                "Autor: " + autor + "\n" +
                "Genero: " + genero + "\n" +
                "Año de publicacion: " + añoPublicacion + "\n");
        if (this.isDisponible()) {
            sb.append("Disponible: Esta disponible \n } ");
        } else {
            sb.append("Disponible: No esta disponible \n }");
        }
        return sb.toString();
    };

    public static class Builder {
        private String titulo;
        private String autor;
        private String genero;
        private int fechaPublicacion;
        private boolean disponilbe;

        public Libro.Builder titulo(String titulo) {
            this.titulo = titulo;
            return this;
        }

        public Libro.Builder autor(String autor) {
            this.autor = autor;
            return this;
        }

        public Libro.Builder genero(String genero) {
            this.genero = genero;
            return this;
        }

        public Libro.Builder añoPublicacion(int fecha) {
            this.fechaPublicacion = fecha;
            return this;
        }
        public Libro.Builder disponible(boolean disponible) {
            this.disponilbe = disponible;
            return this;
        }
        public Libro build() {
            return new Libro(this);
        }

    }
}
