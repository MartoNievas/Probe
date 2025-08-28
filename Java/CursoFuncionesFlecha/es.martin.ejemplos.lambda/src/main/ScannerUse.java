package main;

import java.util.Scanner;

public class ScannerUse {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in); //Podemos pedir inputs por con consola con system.in
        //next funciona como un iterador o una lista por asi decirlo, guarda un registro de dond se encunetra en el i-esimo elemento del dato
        int total = 0;
        while(sc.hasNextInt()) {
            int numeroActual = sc.nextInt();
            total += numeroActual;
            System.out.println("Una palabra: " + numeroActual);
        }
        System.out.println("El total es de: " + total);
    }
}
