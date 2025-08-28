import java.util.List;
import java.util.Random;
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.function.Supplier;

public class ReferenciasMetodos {
    public static void main(String[] args) {

        //Primera forma de hacer referencia (Metodos estaticos).
        Function<String,Long> lambda = (str) -> Long.parseLong(str);
        Function<String,Long> lambdb = Long::parseLong; // con el ::parseLong hacemos refercnia
        // a que metodo estatico queremos usar de la clase de la izquierda.

        //Segunda forma de hacer referencia (Constructores)
        //Supplier<T> es una funcion sin parametros que devuelve algo de tipo T (generico)
        Supplier<Random> generarRandom1 = () -> new Random(); //Esto tiene un paso intermedio
        Supplier<Random> generarRandom2 = Random::new; //Este llama al constructor directamente
        Function<Long,Random> generarRandom3 = (num) -> new Random(num);
        Function<Long,Random> generarRandom4 = Random::new;

        //Tercer forma de hacer referencia (Cuando ya hay una instancia de la clase)

        String cadena = "hola mundo";
        Supplier<Integer> len = () -> cadena.length(); //Otra vez lo mismo se usa un metodo intermedio
        //Se puede simplificar
        Supplier<Integer> len1 = cadena::length;
        //Nuevo tipo Predicate<T> empieza con un Elemento T y devuelve un booleano
        Predicate<String> pred = (str) -> cadena.startsWith(str);
        Predicate<String> pred2 = cadena::startsWith;

        //Cuarta forma de hacer refernecia (Si el metodo no es estatico)
        //Hace falta hacer referencia a la instancia

        // (str) -> str.length(); es equivalente a esto, por eso debemos indicar quien es str
        Function<String,Integer> func1 = String::length;

        //Por ultimo la interfaz grafica BiFunction<> la cual acepta dos parametros de tipo
        //T, U y devuelve uno de tipo R

        BiFunction<String,Integer,String> func2 = String::repeat;

        //ejemplo
        String lista = List.of("continue","break","return").stream().map(func1).toList().toString();
        System.out.println(lista);


        String dato = "Mi nombre es Martin";
        Integer longitud = func1.apply(dato);
        System.out.println(longitud);





    }
;}
