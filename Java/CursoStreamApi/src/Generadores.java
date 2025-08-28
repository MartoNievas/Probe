import java.util.Properties;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class Generadores {
    public static void main(String[] args) {
        Properties props = new Properties();
        props.setProperty("JAVA_HOME","/opt/java/java21");
        props.setProperty("MAVEN_HOME","/opt/maven");
        props.setProperty("CATALINA_HOME","/opt/tomcat8");

        String value = props.values().stream().
                map(String::valueOf).
                collect(Collectors.joining(", "));
        //Mas metodos sobre streams
        //Tenemos muchos metodos para generar streams

        //Para IntStream podemos usar algo que se asemja a un bucle for con rango
        IntStream numStream = IntStream.rangeClosed(10,20).filter(num -> num % 2 == 0);
        numStream.forEach(System.out::println);

        IntStream nums = IntStream.of(1,2,3,4,5,6);
        nums.forEach(System.out::println);
        System.out.println(value);


    }


}
