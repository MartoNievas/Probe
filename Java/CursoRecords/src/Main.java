//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or

// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

//Un record en una clase que agrupa varibles, para poder usarlas meidieante un mismo identificador
public class Main {
    public static void main(String[] args) {
        Cuenta c = new Cuenta("Martin","Martin2004",true);
        System.out.println(c.nombre());
        System.out.println(c.clave());
        c.tienePrivilegios();
        //Setter en un record de java

        Producto p = new Producto("Yerba",2000,10); //Aqui tenemos un prodcuto
        //Pero como hubo inflacion quiero modificar el precio
        System.out.println(p.toString());
        //Modificamos
        Producto q = p.setPrecio(2500);
        System.out.println(q.toString());

        //Por mas que parezcan iguales son dos instancias distintas de la misma clase

        //Ahora vamos a ver que me interesa usar

        Grupo g = new Grupo("Los skibidi", "729");
        int hash = g.hashCode();

        //Creemos unas instancias

        Producto arroz = new Producto("Arroz",1000,0);
        Producto leche = new Producto("leche",700,0);

        Carrito carrito = new Carrito();

        carrito.productos().add(arroz);
        carrito.productos().add(leche);
        System.out.println("Precio: " + carrito.precio());

        //Pero si podemos modifcar la informacion del array del productos debido a que no estamos tocando al
        //registro producto ni al registro carrito

        carrito.productos().clear();
        System.out.println("Precio: " + carrito.precio());
        //Como borramos el contenido del array prodcutos entonces el precio de salida es 0.





    }
}