public record Cuenta(String nombre, String clave, boolean privilegiada) {
    //Los records utilizan informacion inmutable, la unica manera de cambiar la informacion
    //Es cambiando toda la instancia del registro que se esta utilizando
    public String identificador() {
        return "@" + nombre;
    }

    //Tambien un registro puede tener distintos contructores con mas o menos campos
    //Si o si hay que utilizar el construtor original
    public Cuenta(String nombre, String clave) {
        this(nombre,clave,false);
    }

    public void tienePrivilegios() {
        if (this.privilegiada) {
            System.out.println("Tiene privilegios");
        } else {
            System.out.println("Es regular");
        }

    }
}
