import java.io.*;


public class FileOutputYInputStreamUse {
    public static void main(String[] args) {
        OutputStream fs = null; //Instaciamos un objeto de la clase FileOutputStream
        //Bloque try-with
        try {
            fs = new FileOutputStream("datoss.txt");
            for (var i = 0x10; i <= 0x19; i++) {
                fs.write(i);
            }
            fs.flush();
            fs.close(); //sirve para cerrar el OutputStream

        } catch (FileNotFoundException e) {
            e.getMessage();
            System.out.println("Fallo la operacion");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    //Ahora supongamos que tenemos los datos en el disco podemos usar la clase
        //FileInputStream para leer esa informacion y consumirla
        //veamos los metodos read que sirven para leer archivos ojo que pueden tirar
        //excepcion si no se encuentra en el archivo
        try {
            InputStream fs1 = new FileInputStream("datos.txt");
            int val;

            System.out.println("ESTOY AQUI");
            byte[] arr = new byte[8]; //Sigifica que voy a poder leer 8 bytes
            int cuantos = fs1.read(arr);
            System.out.println(cuantos);
            //Lo mismo metodo close sirve para cerrar y liberar el recurso
            cuantos = fs1.read(arr);
            System.out.println(cuantos);
            System.out.println("-----------------------");

            do {
                val = fs1.read();
                if (val != -1) {
                    System.out.println(val);
                } else {
                    System.out.println("FIN");
                }
            } while( val != -1);


        } catch (FileNotFoundException e) {
            e.getMessage();
            System.out.println("Fallo");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        //Sistema try-with ya lo vimos, en la parte del bloque try se le pasa como
        //parametro el recurso que vamos a utilizar y se eso se interpreta como un bloque finally
        //para poder cerrar el recurso al final de la ejecucion del bloque y no poner el bloque finally

        //por ejemplo

        try (FileInputStream fs2 = new FileInputStream("datos")) {
            byte[] arr = new byte[1024];
            int cuantos = fs2.read(arr);
        } catch (IOException e) {
            e.getMessage();
        }




    }
}
