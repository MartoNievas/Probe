//Todos los enumerados tiene como metodos estaticos
public enum PuntoCardinal {
    NORTE('N'),
    SUR('S'),
    ESTE('E'),
    OESTE('O');
    //Tenenmos la posiblidad de intoducir metodos y atributos, pero no podemos uitlizarlos fuera del
    //enumerado

    PuntoCardinal(char sigla) {
        this.sigla = sigla;
    }

    public char getSigla() {
        return sigla;
    }
    public PuntoCardinal girarHorario() {
        switch (this) {
            case ESTE:
                return SUR;
            case SUR:
                return OESTE;
            case NORTE:
                return ESTE;
            case OESTE:
                return NORTE;
        }
        return null;
    }

    private final char sigla;


}
