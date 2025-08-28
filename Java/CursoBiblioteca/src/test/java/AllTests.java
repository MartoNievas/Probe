
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.jeasy.random.EasyRandom;
import org.jeasy.random.EasyRandomParameters;

import java.util.ArrayList;

import static org.junit.jupiter.api.Assertions.*; // Importa estas aserciones
public class AllTests {
    @Test
    void testCreacionLibro() {
        Libro miLibro = new Libro.Builder().titulo("Quijote").autor("Quiroja").
                genero("Fantasia").añoPublicacion(1972).disponible(true).build();
        assertEquals("Quijote",miLibro.getTitulo());
        assertEquals("Quiroja",miLibro.getAutor());
        assertEquals("Fantasia",miLibro.getGenero());
        assertEquals(1972,miLibro.getAñoPublicacion() );
        assertTrue(miLibro.isDisponible());

        //Cambiamos el valor de disponible
        miLibro.setDisponible(false);
        assertFalse(miLibro.isDisponible());

    }
    @Test
    void testsUsuarioDesde0() {
        //Probamos el constructor desde 0
        Usuario user = new Usuario("ejemplo","ejemplo","ejemplo@gmail.com",453);
        Libro miLibro = new Libro.Builder().titulo("Quijote").autor("Quiroja").
                genero("Fantasia").añoPublicacion(1972).disponible(true).build();
        assertEquals("ejemplo",user.getNombre());
        assertEquals("ejemplo",user.getApellido());
        assertEquals("ejemplo@gmail.com",user.getEmail());
        assertEquals(453,user.getIdSocio());
        //Comienza con una lista vacia
        assertEquals(new ArrayList<Libro>(),user.getLibrosPrestados());

        user.addLibro(miLibro);
        ArrayList<Libro> lista = new ArrayList<Libro>();
        lista.add(miLibro);
        assertEquals(lista,user.getLibrosPrestados());

        //Probamos el remove

        user.removeLibro(miLibro);
        assertEquals(new ArrayList<Libro>(),user.getLibrosPrestados());

    }
    @Test
    void testsOtroCons() {
        EasyRandom generador = new EasyRandom();
        Usuario user = generador.nextObject(Usuario.class);



    }



}
