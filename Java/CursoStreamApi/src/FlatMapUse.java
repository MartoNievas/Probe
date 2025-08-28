import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Gatherer;

public class FlatMapUse {
    public static void main(String[] args) {
        var frutas = List.of("Pera","Manzana","Mandarina","Kiwi");
        var variedades = List.of("Verde","Amarilla","Premium");

        //Esto se puede hacer mas facil con el flatmap
        var strings =frutas.stream().
                map((fruta) -> {
                    return variedades.stream()
                            .map((var) -> fruta + " " + var).
                            toList();
                } ).toList();
        System.out.println("Con map..............");
        System.out.println(strings);

        //El metodo flatmap lo que hace es devolverme un unica lista
        //La funcion que se le pasa como parametro debe devolver un stream de tipo R, no como map que devuelve un tipo R
        //Si el stream original tiene orden, el stream de salida tambien lo debe tener
        //Basicamente es la operacion de aplanar combinada con un map

        var strings1 = frutas.stream().flatMap((fruta) ->
                        variedades.stream().map
                                ((var) -> fruta + " " + var)).
                toList();
        System.out.println("Con flatMap.......");
        System.out.println(strings1);

        //Ahora vamos a ver la operacion gather
        //gather sirve para nosotros poder definir nuestra propia operacion intermedia en un linea de streams
        List<String> meses = List.of("enero","febrero","marzo","abril","mayo","junio");
        //Esto es un map y filter al mismo tiempo, que se podria hacer con .filter().map()
        Gatherer.Integrator<Void, String, String> integrator = (_, elem, downstream) -> {
            if (!elem.endsWith("o")) return true;
            if(elem.length() != 5) return true;
            return downstream.push(elem.toUpperCase());
        };

        var traducido = meses.stream().
                gather(Gatherer.ofSequential(integrator)).
                collect(Collectors.toList());
        System.out.println(traducido);
    }

}
