public class Hilo1 extends Thread{

    @Override
    public void run() {
        long i = 0;
        for (;;) {
            boolean inter = isInterrupted();
            if (inter ) {
                System.out.println("El hilo a sido interrunpido");
                return;
            }
            if (++i == 1000000000) {
                i = 0;
                System.out.println("Hola desde hilo 1");
            }
        }
    }
}

