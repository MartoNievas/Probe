import java.io.Serializable;

//Para que el objeto se pueda leer debe implementar la interfaz
//Serializable
//Si se modifica algun atributo del objeto ya no va a ser posible leer el archivo

//Hablemos de la seguridad de Serializable, no sirve para guardar a largo plazo,
//es mas conveniente para guardar informacion a corto plazo, tampoco es muy escalable
//debido a que si se modifica un atributo de una clase la informacion quedaria inutilizable
//Basicamente el problema es leer la informacion, no escribirla

//Serializable fue pensado para hacer transmisiones de datos a corto plazo, por ejemplo
//tines concectos tu ObjectOutputStream y tu ObjectInputStream a la red, para hacer una copia
//de la instancia

//Los problemas de seguridad son seria por ejemplo se le puede pasar cualquier cantidad de bits
//e instanciar clases en una aplicacion

//a ObjectOutputStream no le importa si el campo es privado tiene acceso de todos modos

public class Empleado implements Serializable {
     private String nombre;
     private String apellido;
     private String ocupacion;

    public Empleado(String nombre, String apellido, String ocupacion) {
        this.nombre = nombre;
        this.apellido = apellido;
        this.ocupacion = ocupacion;
    }

    @Override
    public String toString() {
        return "Empleado{" +
                "nombre='" + nombre + '\'' +
                ", apellido='" + apellido + '\'' +
                ", ocupacion='" + ocupacion + '\'' +
                '}';
    }
}
