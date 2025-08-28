import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.function.Predicate;

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
public class Main {
    public static void main(String[] args) {
        //Con la api de stream podemos procesar datos como si fuera un tuberia
        List<Integer> lista = new ArrayList<>();
        lista.add(1);
        lista.add(2);
        lista.add(3);
        System.out.println(lista.toString());
        System.out.println((lista.stream().map((num) -> num * 2)).toList().toString());

        List<List<Integer>> arr1 =  Arrays.asList( Arrays.asList(1,2,3,4), Arrays.asList(1,2), Arrays.asList(1,2,3,4,5,6));
        //Siempre poner el metodo de la interfaz no de la clase
        List<Integer> longitudes = arr1.stream().map(List::size).toList();
        System.out.println("Estas son las longitudes de la lista " + arr1.toString() + " : " + longitudes.toString());

        //Vamos a ver como filtrar por ejemplo
        List<Character> vocales = Arrays.asList('a','e','i','o','u');

        //le ponemos mayusculas y minusculas
        List<String> palabras = Arrays.asList("hola","Andamio","pelicula");

        Predicate<String> verificar = (str) -> {
            if (str.isEmpty()) return false;
            return vocales.contains(str.charAt(0));
        };


        List<String> filtrados = palabras.stream().map(String::toLowerCase).filter(verificar).toList();
        System.out.println(filtrados.toString());

        //Seguimos utilzando la API stream

        List<String> continentes = Arrays.asList("America","Europa","Asia","Oceania","Africa","Antartida","America","asia");

        //Los streams usan genericos Stream<T>
        System.out.println("--------------------");
        continentes.stream()
                .filter((str) -> str.startsWith("A") || str.startsWith("a")) //Esta funcion devuelve un Stream<String>
                .map(String::length) //En cambio map si puede tranformar el tipo de dato del Stream, en este caso Stream<Integer>
                .forEach(System.out::println);
        //Metodos importantes de la clase Stream
        //Skip sirve para saltarse n elementos si n es mayor o igual que la longitud de la coleccion, lo que pasa es que
        //devuleve un arreglo vacio
        //Limit limitamos la cantidad de elementos procesados por la API Stream, si el n es mayor o igual que la longitud de la coleccion
        //devuelve todos
        //Ademas como dato adicional los Stream son de evalucion lazy es decir se evaluan cuando es necesario, no hacen computaciones
        //al pedo por asi decirlo, hay que ponerle un metodo terminal por ejemplo forEach

        //Metodo peak es una funcion parad depurar codigo
        System.out.println("======================");
        continentes.stream().
                peek(str -> System.out.println("Continente: " + str)).
                limit(4).
                forEach(System.out::println);

        System.out.println("////////////////////////////");
        //dropWhile descarta elementos mientras se cumpla una determinada condicion, si no se cumple en un paso deja de descartar
        //de ahi en adelante
        //takeWhile toma elementos mientras se cumpla la condicion, en el momento que no se cumpla descarta todos.
        //sorted ordena un stream, si es un objeto complejo se le puede pasar un Comparator
        //Para usar takeWhile o dropWhile debe estas ordenado el stream, para que se comporte de manera determinista
        continentes.stream().
                sorted().
                takeWhile(str -> str.startsWith("A")).
                forEach(System.out::println);

        //Ultimo metodo distinct lo que hace es elminar repetidos
        System.out.println(".................................");
        continentes.stream().
                distinct().
                forEach(System.out::println);

        //Es importante que al stream se le ponga un operacion terminal, porque al ser lazy, no se evalua hasta que
        //este la misma, toda operacion terminal es aquella la cual no devuelve un stream
        System.out.println("Codigo actual: ");
        continentes.stream().
                distinct().
                filter(s -> !s.equals("asia")).
                sorted().
                forEach(s -> System.out.println("Contienente: " + s));

        var total = continentes.stream().
                filter(s -> s.startsWith("A")).
                count();

        System.out.println("Los continentes que comienzan con A son: " + total);

        List<Integer> numeros = Arrays.asList(32,12,12,4,21,2,3,4,5,6);

        Integer minimo = numeros.
                stream().
                reduce((num, min) -> (num < min) ? num : min).get();
        Integer maximo = numeros.
                stream().
                reduce((num,max) -> (num > max) ? num : max).
                get();

        System.out.println("El valor minimo de " + numeros + " es " + minimo);
        System.out.println(("El valor maximo de " + numeros + " es " + maximo));
    }

}