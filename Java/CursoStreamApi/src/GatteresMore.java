import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.function.BiConsumer;
import java.util.stream.Collectors;
import java.util.stream.Gatherer;

public class GatteresMore {
    public static void main(String[] args) {
        List<Libro> libros = List.of(
                new Libro("Cien años de soledad", "Gabriel García Márquez", 1967),
                new Libro("1984", "George Orwell", 1949),
                new Libro("El Señor de los Anillos", "J.R.R. Tolkien", 1954),
                new Libro("Orgullo y prejuicio", "Jane Austen", 1813),
                new Libro("Matar a un ruiseñor", "Harper Lee", 1960),
                new Libro("Don Quijote de la Mancha", "Miguel de Cervantes", 1605),
                new Libro("Crimen y castigo", "Fiódor Dostoievski", 1866),
                new Libro("Ulises", "James Joyce", 1922),
                new Libro("Moby Dick", "Herman Melville", 1851),
                new Libro("Guerra y paz", "León Tolstói", 1869)
        );
        //Seguimos con el uso de gather
        Gatherer.Integrator<Set<Integer>,Libro,Libro> func = (state, elem, downstream) -> {
            if(state.contains(elem.año())) return true;
            state.add(elem.año());
            return downstream.push(elem);
        };

        //En el initialazer se le pasa un lambda o referencia a un metodo estatico, para crear una instacia del objeto iniciador
        libros.stream().
                gather(Gatherer.ofSequential(HashSet::new,func)).
                forEach(System.out::println);
        //Ahora vamos a ver el uso del finisher, el finisher acepta como parametros El mismo Integrator, pero ahora acepta un BiConsumer
        //Recordando un BiConsumer es una lambada de 2 parametros que no devulve nada, el BiConsumer como primer parametro acepta el estado
        // y como segundo super R, osea el tipo que estamos consuminedo pero su super clase

        BiConsumer<Contador, Gatherer.Downstream<? super String>> finisher = (count,downstream) -> {
            String result = "He visto " + count.cuatos() + " elementos";
            downstream.push(result);
        };

        List<String> meses = List.of("enero","febrero","marzo","abril","mayo","junio");
        //Esto es un map y filter al mismo tiempo, que se podria hacer con .filter().map()
        Gatherer.Integrator<Contador, String, String> integrator = (count, elem, downstream) -> {
            if (!elem.endsWith("o")) return true;
            if(elem.length() != 5) return true;
            count.otro();
            return downstream.push(elem.toUpperCase());
        };

        var traducido = meses.stream().
                gather(Gatherer.ofSequential(Contador::new,integrator,finisher)).
                collect(Collectors.toList());
        System.out.println(traducido);
        //Basicamente el finisher es para agregar algo al final de todas las iteraciones
    }


    }

//Usamos un record (registro) para guardar informacion de libros
record Libro(String titulo, String autor, int año) {}

class Contador {
    int n;
    Contador () {
        n = 0;
    }

    void otro() {
        n++;
    }
    int cuatos() {
        return n;
    }
}