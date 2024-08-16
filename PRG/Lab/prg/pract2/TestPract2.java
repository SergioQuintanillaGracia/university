package pract2;

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
        compareIsPrefix(s[0], s[0]);
                          
        // a empty                          
        compareIsPrefix(s[0], s[6]);
        
        // b empty                  
        compareIsPrefix(s[6], s[0]);
                          
        // a longer than b                  
        compareIsPrefix(s[6], s[1]);
        
        // a and b equally long and a is prefix of b                  
        compareIsPrefix(s[6], s[6]);
        
        // a and b equally long and a is not a prefix of b                  
        compareIsPrefix(s[5], s[6]);
        
        // a shorter than b and a is prefix of b                  
        compareIsPrefix(s[1], s[6]);
        
        // a shorter than b and a is not a prefix of b                  
        // the first character fails
        compareIsPrefix(s[2], s[6]);
        
        // a shorter than b and a is not a prefix of b                  
        // the last character fails
        compareIsPrefix(s[3], s[6]);
        
        // a shorter than b and a is not a prefix of b                  
        // an intermediate character fails
        compareIsPrefix(s[4], s[6]);          
    }
      
    public static void testIsSubstring() {
        String[] s = {"", "rec", "pecur", 
                      "recurso", "remursi", 
                      "123456789", "recursion",
                      "sion", "curs"};
               
        System.out.printf("%8s %12s %20s %10s\n", 
                          "a", "b", "isSubstring(a,b)", "b.contains(a)"); 
   
        // a and b empty
        compareIsSubstring(s[0], s[0]);
        
        // a empty                          
        compareIsSubstring(s[0], s[1]); 
        
        // b empty                  
        compareIsSubstring(s[1], s[0]); 
        
        // a longer than  b                  
        compareIsSubstring(s[3], s[2]);
        
        // a and b equally long and a is contained within b
        compareIsSubstring(s[3], s[3]);
        
        // a and b equally long and a is not contained within b
        compareIsSubstring(s[3], s[4]);
        
        // a shorter than b and a is contained within b because a is prefix of b
        compareIsSubstring(s[1], s[6]); // Using "rec" and "recursion", where "rec" is a prefix of "recursion"
        
        // a shorter than b and a is contained within b because a is suffix of b
        compareIsSubstring(s[7], s[6]); // Using "sion" and "recursion", where "sion" is a suffix of "recursion"
        
        // a shorter than b and a is contained within b because a is within b from an intermediate position
        compareIsSubstring(s[8], s[6]); // Using "curs" and "recursion", where "curs" is in the middle of "recursion"
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