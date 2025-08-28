import java.util.Iterator;
import java.util.List;

public class IteratorUse {
    //Interfaz Iterator y patron Iterator
    public static void main(String[] args) {
        var lista = List.of("martin", "far", "theo");

        Iterator<String> it = lista.iterator();

        while(it.hasNext()) {
            String x = it.next();
            System.out.println(x);
        }
        for ( String str: lista) {
            System.out.println(str);
        }
    }
}
