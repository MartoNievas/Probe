import java.util.List;
import java.util.Optional;
import java.util.stream.Stream;

public class Streams {
    public static void main(String[] args) {
        List<String> paises = List.of("Argentina","Chile","Colombia","Mexico","Peru","Nicaragua");

        Stream<String> str = paises.stream();

        //Vamos a ver las 3 formas de usar el metodo reduce (Basicamente foldr)
        //Reduce en un metodo terminal

        Integer sumaLongitudes = str.
                reduce(0,(p1,p2) -> p1 + p2.length(), Integer::sum);
        //Una forma de no usar el ultimo metodo es usando un map o filter previo por ejemplo asi
        Integer sumaLongitudes1 = str.
                map(String::length).
                reduce(0, Integer::sum);

        System.out.println("La suma de las longitudes de las plabras es: " + sumaLongitudes1);
        //En general el reduce es preferible evitarlo y usar collect.
        //Porque esta funcion queda mas facil si hacemos
        int sumaLongitudes2 = str.
                map(String::length).mapToInt(n -> n).sum();

        //Observaciones si no le ponemos un identity el tipo que devuelve reduce es Optional<T>

        //Con .get() obtenemos el valor de Integer OJO si la coleccion esta vacia esto falla
        Optional<Integer> sumaLongitudes3 = str.
                map(String::length).
                reduce(Integer::sum);




    }
}
