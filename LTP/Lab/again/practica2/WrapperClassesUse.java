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
        double d = new Double(1.234);
        char c = new Character('a');
            
        // Writing elementary variables
        System.out.println("int i = " + i);
        System.out.println("double d = " + d);
        System.out.println("char c = " + c);
               
        // Assignment of elementary values to wrapper variables
        Integer eI = 123456;
        Double dI = 1.234;
        Character cI = 'a';
            
        // Writing wrapper variables
        System.out.println("Integer I = " + eI);
        System.out.println("Double I = " + dI);
        System.out.println("Character I = " + cI);
    }    
}