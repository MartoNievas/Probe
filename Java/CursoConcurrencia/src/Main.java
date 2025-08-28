//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

//Concurrencia, Multitarea y Paralelismo en java
//Que es un hilo? Un hilo es la base de la concurrencia, es un contexto de ejecucicon de un programa
//Un hilo tiene un estado determino independiete entre si (por lo general y asi deberia ser)

//Maneras de crear hilos
//1 extender de Thread (no es escalable a largo plazo)
//2 implementar la interfaz de Runnable (Esto es lo mejor)
public class Main {
    public static void main(String[] args) {
        //La clase Thread tiene muchos getters y setters
        Hilo1 h1 = new Hilo1();
        Thread h2 = new Thread(new Hilo2());

        //El orden importa
        h1.start(); //El metodo para lanzar un hilo es start no run
        h2.start();

        try {
            //El metodo estatico sleep lanza un InterrumptedException

            Thread.sleep(4000); //Se puede poner en milisegundo o usar la Clase Duration con el metodo
            //ofSeconds para ponerlo directamente en segundos
        } catch (InterruptedException e) {
            e.getMessage();
        }
        h1.interrupt(); //Interrumpimos la ejecucion del hilo 1



        //Otra propiedad que forma parte de un hilo es el estado de interrupcion, es un booleano que indica si
        //el hilo fue interrumpido o no

    }
}