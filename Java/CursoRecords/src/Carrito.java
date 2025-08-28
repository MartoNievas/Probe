import java.util.List;
import java.util.ArrayList;
public record Carrito(List<Producto> productos) {
    public Carrito() {
        this(new ArrayList<>());
    }

    public int cantidad() {
        return productos.size();
    }

    public int precio() {
        return productos.stream().mapToInt(Producto::precio).sum();
    }
}
