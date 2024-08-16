package examExample;

import java.util.Scanner;
import assist.TimeInstant;
/**
 * Clase TestTIExample: clase programa que usa la clase TimeInstant 
 * del paquete assist, identica a tu clase TimeInstant con el metodo 
 * toMinutes() correcto.  
 * 
 * @author IIP
 * @version Curso 2023-24
 */
public class TestTIExample {
    // No se usan objetos de esta clase
    private TestTIExample() { }
    
    public static void main(String[] args) {
        Scanner teclado = new Scanner(System.in);
        System.out.println("Lectura de teclado de una hora.");
        System.out.print("   -> Introduzca las horas (entre 0 y 23): ");
        int h = teclado.nextInt();
        System.out.print("   -> Introduzca los minutos (entre 0 y 59): ");
        int m = teclado.nextInt();
        // Una vez leidos los datos de horas y minutos desde el teclado y 
        // suponiendo que son correctos:
        // a) Crea un TimeInstant ti a partir de dichos datos,
        TimeInstant ti = new TimeInstant(h, m);
        
        // b) Muestra por pantalla ti (en el formato "hh:mm") 
        //    y su equivalente en minutos
        System.out.println(ti);
        System.out.println(ti.toMinutes());
    }
}