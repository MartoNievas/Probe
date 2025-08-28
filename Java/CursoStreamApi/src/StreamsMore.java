import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Scanner;
import java.util.stream.Stream;
public class StreamsMore {
    //Ahora vamos a ver LongStream, DoubleStream e IntStream
    //Tienen muchos metodos especializados en
    public static void main(String[] args) {
        System.out.println("Ingrese el path del archivo o directorio: ");
        Scanner input = new Scanner(System.in);

        String url = input.next();

        try (Stream<Path> fs = Files.list(Paths.get(url))){

            long total = fs.peek(p -> System.out.println(p.getFileName())).
            map(Path::toFile).mapToLong(File::length).
            sum();
            System.out.println("La suma total de bytes del directorio es: " + total + " Bytes" );
        } catch (IOException e) {
            e.getMessage();
            System.out.println("Fallo");
        }
        input.close();
    }
}
