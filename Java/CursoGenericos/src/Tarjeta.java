import java.awt.List;
import java.util.ArrayList;

public abstract class Tarjeta {
    private Integer fechaVencimiento;
    private String nombre;
    private String apellido;
    private Integer numero;

    public Integer getFechaVencimiento() {
        return fechaVencimiento;
    }

    public String getApellido() {
        return apellido;
    }

    public String getNombre() {
        return nombre;
    }

    public Integer getNumero() {
        return numero;
    }
    public void nada(){
      ArrayList<Integer> list = new ArrayList<>(); 
    }
}
