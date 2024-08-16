package pract2_2;

/**
 * Clase TestPract2: program class for checking 
 * method isPrefix(String, String) and isSubstring(String, String)
 * 
 * @author PRG - ETSINF - DSIC - UPV
 * @version Academic Year 2023/2024
 */
public class TestPract2 {
    /** Hidden constructor */
    private TestPract2() { }
    
    public static void testIsPrefix() {
        String[] s = {"", "rec", "pecur", 
                      "recurso", "remursi", 
                      "123456789", "recursion"};
                      
        System.out.printf("%8s %12s %20s %12s\n", 
            "a", "b", "isPrefix(a, b)", "b.startsWith(a)");
            
        // a and b empty
        // To complete 
                          
        // a empty                          
        // To complete 
        
        // b empty                  
        // To complete 
                          
        // a longer than b                  
        // To complete 
        
        // a and b equally long and a is prefix of b                  
        // To complete 
        
        // a and b equally long and a is not a prefix of b                  
        // To complete 
        
        // a shorter than b and a is prefix of b                  
        // To complete 
        
        // a shorter than b and a is not a prefix of b                  
        // the first character fails
        // To complete 
        
        // a shorter than b and a is not a prefix of b                  
        // the last character fails
        // To complete 
        
        // a shorter than b and a is not a prefix of b                  
        // an intermediate character fails
        // To complete              
    }
      
    public static void testIsSubstring() {
        String[] s = {"", "rec", "pecur", 
                      "recurso", "remursi", 
                      "123456789", "recursion",
                      "sion", "curs"};
               
        System.out.printf("%8s %12s %20s %10s\n", 
                          "a", "b", "isSubstring(a,b)", "b.contains(a)"); 
   
        // a and b empty
        // To complete  
        
        // a empty                          
        // To complete  
        
        // b empty                  
        // To complete  
        
        // a longer than  b                  
        // To complete  
        
        // a and b equally long and a is contained within b
        // To complete  
        
        // a and b equally long and a is not contained within b
        // To complete  
        
        // a shorter than b and a is contained within b
        // because a is prefix of b
        // To complete  
        
        // a shorter than b and a is contained within b
        // because a is suffix of b
        // To complete  
        
        // a shorter than b and a is contained within b
        // because a is within b from an intermediate position
        // To complete  
    }
     
    private static void compareIsPrefix(String a, String b) {
        System.out.printf("%12s %12s %12b %12b\n", a, b, 
                          PRGString.isPrefix(a, b),
                          b.startsWith(a));
                          
        
    }
    
    private static void compareIsSubstring(String a, String b) {
        System.out.printf("%12s %12s %12b %12b\n", a, b, 
                          PRGString.isSubstring(a, b),
                          b.contains(a));
    }
   
}
