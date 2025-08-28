package main;

public class Perro extends Animal{
    //Override es para evitar problemas, sirve para extender el metodo o sobreescribir el metodo de la super clase a
    // la subclase.

    @Override
    public void sonido(){
        System.out.println("El perro hace gua gua");
    }
}
