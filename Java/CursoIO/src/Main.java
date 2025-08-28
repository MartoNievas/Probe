import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Main {

    public static void escribirObjetoc() {
        try (OutputStream fos = new FileOutputStream("Objetos.txt");
             ObjectOutputStream oos = new ObjectOutputStream(fos);) {
            List<String> elemetos = new ArrayList<>();
            elemetos.add("Hola");
            elemetos.add("Adios");
            elemetos.add("Buenas");

            double x = 3.5;
            oos.writeDouble(x);
            oos.writeBoolean(true);
            oos.writeUTF("Hola mundo"); //Nos sirve para escribir string
            oos.writeObject(elemetos); //a writeObject le puedo pasar casi cualquier objeto para escribirlo en un archivo
        } catch (IOException e) {
            e.getMessage();
        }
    }
    public static void leerObjetos() {
        try (InputStream fis = new FileInputStream("Objetos.txt")
        ;ObjectInputStream ois = new ObjectInputStream(fis)) {
            System.out.println(ois.readDouble());
            System.out.println(ois.readBoolean());
            System.out.println(ois.readUTF()); //Nos sirve paraleer un string, pero no es tan recomendado
            //por la forma en que lo lee.
            System.out.println(ois.readObject()); //Con readObject podemos leer casi cualquier objeto
        } catch (IOException e) {
            e.getMessage();
        } catch (ClassNotFoundException e) {
            e.getMessage();
        }
    }
    static void escribirEmpleados() {
        var empleados = new ArrayList<Empleado>();
        empleados.add(new Empleado("Martin","Nievas","Programador"));
        empleados.add(new Empleado("Lucia","Perez","Contadora"));
        empleados.add(new Empleado("Roberto","Kun","Abogado"));
        empleados.add(new Empleado("Maria","Suarez","Secretaria"));

        try (var fos = new FileOutputStream("empleados.txt");ObjectOutputStream oos = new ObjectOutputStream(fos)) {
            oos.writeObject((empleados));
        } catch (IOException ex) {
            Logger.getLogger((ObjectOutputStream.class.getName())).log(Level.SEVERE,null,ex);
        }
    }

    static void leerEmpleados() {
        List<Empleado> lista = new ArrayList<>();
        try (var fis = new FileInputStream("empleados.txt");
             ObjectInputStream ois = new ObjectInputStream(fis)) {
            lista = (ArrayList<Empleado>) ois.readObject();
        } catch (IOException ex) {
            Logger.getLogger((ObjectOutputStream.class.getName())).log(Level.SEVERE,null,ex);

        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
        for(Empleado e: lista) {
            System.out.println(e);
        }

    }
    //Vamos con BufferedInputStream y BufferedOutputStream
    //Con estas clases podemos leer mas informacion de golpe, cuando llamemos a read lo que
    //va a ocurrir va a llamar al Stream envuelto para que leea un monton de informacion y eso lo
    //guarda en un buffer y lo que hace el BufferedInputStream es proporcionarnos el Byte que solicitamos
    //La gracia es que la proxima vez que llamemos a read si queda informacion en el buffer
    //la operacion va a ser muchisimo mas rapida

    //Esto es muy poco eficiente estamos llamando 256 veces a write, si usamos
    //BufferedOutputStream
    static void escribirNumeros() {
        try(FileOutputStream fos = new FileOutputStream("numeros.bin");
        BufferedOutputStream bos = new BufferedOutputStream(fos);) {
            for (int i = 0; i <= 255; i++) {
                bos.write(i);
            }
            bos.flush();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    //en este ejemplo vemos que podemos envolver multiples Streams
    static void escribirCadenas() {
        try(FileOutputStream fos = new FileOutputStream("cadenas.bin");
            BufferedOutputStream bos = new BufferedOutputStream(fos);
            ObjectOutputStream oos = new ObjectOutputStream(bos)) {
            for (int i = 0; i <= 255; i++) {
                oos.writeUTF("Hola como andas German " + i + "\n");
            }
            oos.flush(); //El flush se pone para asegurarse de que no quede informacion sin escribir en memoria

        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    //Vamos con BufferedInputReader y reset
    //reset y mark sirve para dejar acumulado una serie de bytes y poder volver en el tiempo

    //Si nunca se llama a mark y se usa reset el programa lanza un excepcion
    //Ojo porque si el buffer no es lo suficientemente grande y se hace una llamada con reset se puede desbordar lanzando una excepcion

    static void leerNumeros() {
        try (FileInputStream fis = new FileInputStream("numeros.bin");
        BufferedInputStream bis = new BufferedInputStream(fis);) {
            int octeto;
            int retorno = 0;

            bis.mark(100); //creo un marcador al principio del InputStream

            do {
                octeto = bis.read();
                //bis.mark(50); //esto dice que los proximos 50 bytes que se lean, los va a guardar
                //bis.reset(); //aqui lo que hago es volver a la posicion en la que estaba mi InputStream la ultima vez que se llamo a mark
                if (octeto != -1) {
                    System.out.println(octeto);
                }
                if (octeto % 20 == 0) {
                    System.out.println();
                }
                if(octeto == 50 && ++retorno < 4) {
                    bis.reset(); //con esto vuelvo a la ultima vez donde llame a mark
                }
            }while (octeto != -1) ;
        } catch (IOException e) {
            e.getMessage();
        }
    }

    //Vamos con Reades y Writer, hace algo similar a OutputStream e InputStream pero trabaja con caracteres
    static void escribir() {
        //Este archivo va a ser de tipo texto
        try (Writer w = new FileWriter("ejemplo.txt")) {
            w.append("Hola mundo\n"); //Acepta Strings
            w.write("Como estas\n");
            w.append("Chau munndo\n"); //Un String es un tipo de CharSequence
            //w.close(); Para cerrar
            w.flush(); //Para empujar cualquier cosa hacia el buffer definitivo
        } catch (IOException e) {
            e.getMessage();
        }
    }

    static void leer() {
        try (Reader r = new FileReader("ejemplo.txt");
        BufferedReader br = new BufferedReader(r);) {
            String ch = null;
            do {
                ch = br.readLine();
                if (ch != null) {
                    System.out.println(ch);
                }
            } while (ch != null);
        } catch (IOException e) {
            e.getMessage();
        }
    }

    //Con InputStreamReader tranformamos un InputStream en un Reades eso nos permite leer texto
    //Con OutputStreamWriter transformamos un OutputStream en un Writer eso nos permite escribir texto
    static void prueba() {
        try (Reader r = new InputStreamReader(System.in);
        BufferedReader br = new BufferedReader(r)) {
            String ch = null;
            do {
                ch = br.readLine();
                if (ch != null) {
                    System.out.println("> " + ch + " " + ch.length() + " <");
                }
            } while (ch != null);
        } catch (IOException e) {
            e.getMessage();
        }
        try (Writer w = new OutputStreamWriter(System.out);) {
            w.write("Hola como estamos");
        }catch (IOException e) {
            e.getMessage();
        }
    }
    //Por ultimo PrintStream sirve para cuando queremos escribir masivamente en archivos
    static void prints() {
        try (PrintWriter pw = new PrintWriter("impresora.txt")) {
            pw.println(25);
            pw.write("hola mundo");
            pw.flush();

        } catch (IOException e) {
            e.getMessage();
        }
    }


    public static void main(String[] args) {
        prints();
    }
    //La gracia de ObjectInupot/OutputStream es que podemos leer y escribr cualquier objeto que se pueda escribir
}
//Empezemos con Streams de salida y entrada
// System.in y System.out se llama salida y entrada estandar
//En algunas ocaciones necetiamos otro tipo de entrda que no sea la estandar
//La entrada y la salida puede ser cualquier cosa, un archivo, un servidor por medio de internet, etc.

//Para este proposito vamos a tener una seria de clases que forman parte del paquete java.io
//Los streams se van a comportar como rios que transportan informacion en bytes, la cual va a poder ser consumida
// es decir utilizada para ser modificada o como informacion que podemos enviar hacia afuera

//Dividimos en InputStream que son para el consumo y lo OutputStream para enviar informacion hacia afuera
//Por ejemplo para OutputStream tenemos FileOutputStream que va a servir para escribir datos en archivos
//Tambien tenemos OutputStreams que son envolventes es decir que devuelven otro Stream, es por asi decirlo como un adaptador
