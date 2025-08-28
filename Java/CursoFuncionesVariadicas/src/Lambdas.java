import java.util.function.Consumer;

public class Lambdas {
    //Las lambdas en java son azucar sintatico
    //Es decir realmente son clases anonimas las cuales implementan una interfaz funcional
    //Vamos a ver como hacer funciones que acepten lambdas, es decir tiene que aceptar la interfaz funcional

    public static void foreach(int[] arr, Consumer<Integer> cont) {
        for(int i : arr) {
            cont.accept(i);
        }
    }


    public static void main(String[] args) {
        //declaramos el array
        int[] arr = new int[] {1,2,3,4,5,6,7};
        foreach(arr, (i) -> System.out.println(i));
    }
}
