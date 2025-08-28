//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
public class Main {
    //Con los 3 puntos indicamos que es una funcion variadica.
    //Discretamente valores se interpreta como un array entonces tiene todos sus metodos.
    //Esto es para evitar la sintaxis del arra, esto es mas limpio.
    public static int sumatorio(int... valores) {
        int sum = 0;
        for(int num : valores) {
            sum += num;
        }
        return sum;
    }
    //Suena raro pero tambien se le puede pasar un array como parametro.
    public static void main(String[] args) {
        System.out.println(sumatorio(1,2,3,4,5,6));
        int[] valores = { 1, 2, 3, 4, 5, 6};
        int suma = sumatorio(valores);
        System.out.println(suma);
    }
}