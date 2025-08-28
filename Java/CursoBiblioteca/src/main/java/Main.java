import org.jeasy.random.EasyRandom;
import org.jeasy.random.EasyRandomParameters;


class Main {
    public static void main(String[] args) {
        EasyRandom generador = new EasyRandom();
        Biblioteca biblioteca = generador.nextObject(Biblioteca.class);

        biblioteca.listarLibrosDisponibles();
    }
}
