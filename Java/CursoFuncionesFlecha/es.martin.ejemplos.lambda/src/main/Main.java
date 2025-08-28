package main;

import java.util.Random;

public class Main {

  public static void main(String[] args) {
    Comando ayuda = new AyudaComando();


    //Aqui Comando es una clase anonima por lo tanto se crea el objeto Comando y
      // Se lo asigna a la variable opcciones pero no se puede reutilizar la clase
      // debido a que es anonima
    Comando opciones = new Comando() {

        @Override
        public String nombre() {
            return "opcciones";
        }

        @Override
        public String descripcion() {
            return "muestra las opcciones del programa";
        }
    };
    //Tambien podemos hacer clases anonimas de clases abstractas de la misma manera
      AbstractComando opcionesAbs = new AbstractComando() {
          @Override
          public String nombre() {
              return "";
          }

          @Override
          public String descripcion() {
              return "";
          }

          @Override
          public boolean deprecado() {
              return false;
          }
      };
      // Se puede usar para definir subclases por ejemplo con random
      Random rand = new Random() {
         @Override
         public double nextDouble() {
             return super.nextDouble();
         }
      };
    // Usos de las clases anonimas, para hacer implementaciones de interfaces de un solo
      // uso, o las funciones anonimas de java (Arrow functions).
  }
} 
