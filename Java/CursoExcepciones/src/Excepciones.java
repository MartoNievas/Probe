import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.logging.Logger;
//Basicamente existen 2 tipos de excepciones las controladas las cuales son las que
//extienden de la super clase Exception, son generalmente por pasar parametros errones al metodo
//Y las no controladas que extienden de RunTimeException es decir en tiempo de ejecucion
//Estos errores se deben a mala programacion debido a que no se estan haciendo las validaciones
//correctas.
public class Excepciones {
    public static void metodo1(){
        try {
            Path archivo = Paths.get("documento.txt");
            String contenido = Files.readString(archivo);
            System.out.println(contenido);
        } catch (IOException e) {
            Logger.getLogger(Excepciones.class.getName());
        } finally {
            System.out.println("Esto se ejecuta siempre");
        }

    }

    public static int metodo2() {
        int numerador = 10;
        int denominador = 0;
        if (denominador == 0) {
            throw new ArithmeticException("Estas dividiendo por 0");
        }
        return numerador / denominador;
    }

    public static void main(String[] args) {
        metodo2();
    }


}
