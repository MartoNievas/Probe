import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
public class Main {
    public static void main(String[] args) throws IOException {
        String line = leerArchivo();
        System.out.println(line);
    }

    //Se utiliza principalmente cuando queremos liberar recursos es decir
    //cerrar archivos, etc
    public static String leerArchivo() throws IOException {
        //Usando los parentesis cerramos todos los recursos que declaramos en el mismo
        //Los recursos inicializados deben implementar la interfaz Closeable
        try (FileReader reader = new FileReader("documento.txt")) {
            BufferedReader buffer = new BufferedReader(reader);
            String line = buffer.readLine();
            return line;
        } catch (IOException e) {
            e.getCause();
            e.getMessage();
            System.out.println("Fallo");// Aqui si el archivo no existe puede fallar entonces hay que usar un bloque try catch
        }
        return null;
    }
}
    // pero podemos usar try with recourses
    //Errores que no cometer al usar excepciones
//Numero 1:
// Tener cuidado al poner returns en el bloque finally debido a que tiene prioridad por
// sobre el catch
//Numero 2:
//No utilizar la excepciones por defecto es decir Exception o RuntimeException
// ni poner catch (Exception e) {} o catch (RuntimeException e) {}
//Numero 3:
//No confundir el tipo de excepciones.

