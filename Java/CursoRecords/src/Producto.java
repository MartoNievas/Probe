public record Producto(String nombre, int precio, int stock ) {
    //Como vimos anteriormente la informacion de los registros es inmutable, pero se pueden hacer
    //setters para poder modificar la informacion, mediante crear una copia de la instancia
    //actual con el atributo a modificar

    //Mediante crear copias de la instnacia actual podemos modificar un campo especifico
    //del registro
    public Producto setNombre(String nombre) {
        return new Producto(nombre,this.precio,this.stock);
    }

    public Producto setPrecio(int precio) {
        return new Producto(this.nombre,precio,this.stock);
    }

    public Producto setStock(int stock) {
        return new Producto(this.nombre,this.precio,stock);
    }

    @Override
    public String toString() {
        return "nombre: " + nombre + " precio: " + precio + " stock: " + stock;
    }
    //En escencia un record es un DTO (Data Transfer Object). Es una estructra para poder
    //transportar informacion de una parte a otra
}
