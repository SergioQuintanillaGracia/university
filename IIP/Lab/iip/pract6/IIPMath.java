package pract6;


/**
 * Write a description of class IIPMath here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class IIPMath {
    /** Calculate the sine of an angle x, in radians, *
    * with a maximum error epsilon */
    public static double sin(double x, double epsilon) {
        int i = 1;
        x = x % (2 * Math.PI);
        double sum = x;
        double term = x;
        double prevTerm = x;
        
        while (Math.abs(term) >= epsilon) {
            term = -Math.pow(x, 2) / (2 * i * (2 * i + 1)) * prevTerm;
            sum += term;
            prevTerm = term;
            i++;
        }
        
        return sum;
    }
    
    public static double sin(double x) {
        return sin(x, 0.000000000000001);
    }
    
    /** Write to standard output, line by line, the values of
    * Math.sin(x) and IIPMath.sin(x), for x from 0 to 20 radians.
    * Display the difference in absolute value between these values. */
    public static void table() {
        System.out.println("----------------------------------------------------------------------");
        System.out.println("  x       Math.sin(x)          IIPMath.sin(x)            |dif|");
        System.out.println("----------------------------------------------------------------------");
        
        for (int i = 0; i < 21; i++) {
            double mathSin = Math.sin(i);
            double classSin = sin(i);
            double diff = Math.abs(mathSin - classSin);
            String res = "%2d    %18.15f    %18.15f    %18.15f".formatted(i, mathSin, classSin, diff);
            
            System.out.println(res);
        }
    }
}
