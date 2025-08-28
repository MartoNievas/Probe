public class ExecptionUse {
    //Excepciones y como manejarlas
    public static int calcular() {
        int k = 5 / 0;
        return k + 10;
    }

    //Si usamos el try, el programa no termina de forma prematura si no que termina
    // debido a que se atrapa la excepcion.
    public static void main(String[] args) throws Exception {
        try {
            int res = calcular();
            System.out.println(res);
        } catch (ArithmeticException e) {
            System.out.println("Algo no ha ido bien");
            System.out.println(e.getMessage());
        }

        System.out.println("Fin del programa");
        //Ahora vamos a ver como tirar excepciones
        String codificacion = "clicks = 30, paginas = 10, sesiones = 5";
        try {
            enviarAlServidor(codificacion);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }


    }

    private static void enviarAlServidor(String codificacion) throws Exception{
        if (codificacion == null || codificacion.isEmpty()) {
            Exception excep = new Exception("No me has pasado una codificacion valida");
            throw excep;
        }
    }

}