import java.util.*;

public class Comparadores {
    public static void main(String[] args) {
        int[] arr = new int[] {3,2,3,3,2,42,67,6,311, -1};
        //Los colecciones de tipos de datos primivitivos tienen definidos los metodos de ordena
        //ordenamiento .sort

        //por ejemplo si quiero ordeanar arr basta con:
        Arrays.sort(arr);
        //Asi mostramos la lista ordenada.
        for (int j : arr) System.out.print(j + " ");
        System.out.println(" ");

        //Por ejemplo si declaro un array de personas no puedo usar el metodo sort ya que
        //persona es un tipo de dato complejo o mas bien un objeto entonces tengo que usar
        //un metodo de comparacion. Para eso entras los comparators

        List<Personas> lista = Collections.emptyList(); // declaramos una lista vacia no importa

        //Y ahora veamos como ordenarla.

        //Aqui utilizo una funcion lambda para ordenar con el criterio de menor edad.
        lista.sort((p1,p2) -> p1.getEdad() - p2.getEdad());

        //Podriamos usar el metodo comparingInt de la clase Comparator.
        lista.sort((Comparator.comparingInt(Personas::getEdad)));





    }
}
