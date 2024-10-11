package practica2;

/**
 * class WrapperClassesUse.
 * 
 * @author LTP 
 * @version 2020-21
 */

public class WrapperClassesUse {        
    public static void main(String[] args) {            
        // Assignment of wrapper variables to elementary types 
        int i = new Integer(123456);        
        double d = new Double(1.23456);
        char c = new Character('a');
            
        // Writing elementary variables
        System.out.println("int i = " + i);
        System.out.println("double d = " + d);
        System.out.println("char c = " + c);
               
        // Assignment of elementary values to wrapper variables
        Integer I = 123456; 
        Double D = 1.23456;
        Character C = 'a';
            
        // Writing wrapper variables
        System.out.println("Integer I = " + I);
        System.out.println("Double D = " + D);
        System.out.println("Character C = " + C);
    }    
}
