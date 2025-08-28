public class Triangulo {
    public float base, altura;
    public CalculadoraArea area;
    //Las inners class sirven para declarar una clase de uso especifico, aislar codigo
    //las inners class son inacsesibles.
    public Triangulo(float base, float altura) {
        this.altura = altura;
        this.base = base;
        this.area = new CalculadoraArea();
    }

    @Override
    public String toString() {
        return  "Triangulo{" + "base=" + base + ", altura= " + altura + '}';
    }

    public void print() {
        System.out.println(toString());
    }

    public float CalcularArea() {
        return this.area.area;
    }

    private class CalculadoraArea {
        public float area;

        public CalculadoraArea() {
            this.area = (base * altura);
        }
        @Override
        public String toString() {
            return "CalculadoraArea{" + "area=" + area + '}';
        }
        public void print(){
            System.out.println(toString());
        }
    }
}
