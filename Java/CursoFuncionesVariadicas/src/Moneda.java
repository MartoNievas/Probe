// Vamos a ver como se implementa la interfaz comparable, es util para cosas que tienen
// un orden natural como en este caso una moneda

public class Moneda implements Comparable<Moneda>{
    public final int pesos;

    public final int centavos;

    public Moneda(int pesos, int centavos) {
        if (centavos < 0 || centavos >= 100) {
            throw new IllegalArgumentException("Cenvatovos van en el rango [0, 99]");
        }
        this.pesos = pesos;
        this.centavos = centavos;
    }
    @Override
    public String toString() {
        return pesos + "." + centavos + " ARG";
    }
    //Cosa importante el metodo compareTo si se le pasa un valor null debe fallar
    //debe arrojar una NullPointerException.
    @Override
    public int compareTo(Moneda moneda) throws NullPointerException {
        if (moneda == null) {
            throw new NullPointerException("moneda no es un valor valido");
        }
        return (this.pesos > moneda.pesos) ? 1 : ((this.pesos < moneda.pesos) ? -1 : (
                Integer.compare(this.centavos, moneda.centavos)
                ));
    }

    public static void main(String[] args) {
        Moneda m1 = new Moneda(100,20);
        Moneda m2 = new Moneda(100,20);
        Moneda m3 = null;
        int comparacion = 10000;
        try {
            comparacion = m1.compareTo(m2);
        } catch (NullPointerException e) {
            System.out.println(e.getMessage());
            System.out.println("El puntero esta vacio");
        }
        //Comparacion (como int es primitivo no nesecito el .equals()).
        if (comparacion == 1) System.out.println("m1 es mayor que m2");
        else if (comparacion == -1) System.out.println("m1 es menor que m2");
        else if (comparacion == 0) System.out.println("m1 es igual a m2");
        else System.out.println("Error. Valor de la monera invalido");

        String valorm1 = m1.toString();
        String valorm2 = m2.toString();

        System.out.println(valorm1);
        System.out.println(valorm2);
    }

}
