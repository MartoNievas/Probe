public class Enums {
    //Concepto de tipo enumerado
    private  static PuntoCardinal direccion;
    //Son en el fondo clases que nos permiten acotar valores posibles para una variable
    //Por ejemplo dias de la semana, meses de un anio, etc.
    //Tambien se puede comparar.

    public static void main(String[] args) {
        direccion = PuntoCardinal.SUR;
        var d2 = PuntoCardinal.valueOf("NORTE");
        if (direccion == d2) {
            System.out.println("POS CLARO");
        }
        // El metodo ordinal devuelve el i-esimo enumerado.
        //Ojo que si el orden los enumerados cambia, el numero de ordinal tambien lo hace
        // Eso potencialmente puede generar errores en el codigo.
        System.out.println(direccion.ordinal());
    }
    public Enums() {
        direccion = PuntoCardinal.ESTE;
        this.direccion.getSigla();
        if (direccion == PuntoCardinal.OESTE) {
            System.out.println("Estas mirando hacia el Oeste...");
        }
        PuntoCardinal.valueOf("ESTE");
    }

}
