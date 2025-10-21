package pract5;

import java.io.File;
import java.util.Scanner;
import java.io.FileNotFoundException;
import pract4.utilPRG.CorrectReading;
/**
 * TextComparing class. Program class that calculates the union 
 * or intersection of the words of certain texts.
 *
 * @author (PRG. ETSINF. UPV)
 * @version (Academic Year 2023/24)
 */
public class TextComparing {
    
    private final static String DELIMITERS = 
                      "[\\p{Space}\\p{Punct}\\p{Digit}¡¿]+";
                      
    private TextComparing() { }                  
               
    /**
     * The user is requested to provide the names of two text files,  
     * after which the intersection or union of the sets of words in
     * the texts is performed. 
     * 
     * The result is then displayed in the standard output.
     */
    public static void main(String[] args) {  
        Scanner keyB = new Scanner(System.in);
        System.out.print("First file name: ");
        String nF1 = keyB.next();
        System.out.print("Second File Name: ");
        String nF2 = keyB.next();
        Scanner s1 = null; Scanner s2 = null;
        try {
            // Opening of the files:
            s1 = new Scanner(new File("/home/sergio/Documents/Local University/PRG/prg/pract5/" + nF1));
            s2 = new Scanner(new File("/home/sergio/Documents/Local University/PRG/prg/pract5/" + nF2));
            s1.useDelimiter(DELIMITERS);
            s2.useDelimiter(DELIMITERS);
            // Reading the sets from files:
            SetString cS1 = SetString.setReading(s1);
            SetString cS2 = SetString.setReading(s2);

            // Reading the option and processing the sets
            int option = menu(keyB); // helper method for selecting the operation
            switch(option) {
                case 1:
                    SetString intersection = cS1.intersection(cS2);
                    System.out.println(intersection);
                    break;
                case 2:
                    SetString union = cS2.union(cS2);
                    System.out.println(union);
                    break;
            }

        } catch (FileNotFoundException e) {
            System.out.print("Bad File Access: " + e.getMessage());
        } finally {
            if (s1 != null) { s1.close(); }
            if (s2 != null) { s2.close(); }
        }
    }   
   
    /**
     * Displays a menu of options on the screen and read a valid option from the keyboard.
     * @param kb Scanner, the keyboard.
     * @return int, the valid option.
     */
    private static int menu(Scanner kb) {        
        String msg = "======================================\n"
            + "OPTIONS: \n"
            + "   1. Intersection of the sets of words.\n"
            + "   2. Union of the sets of words.\n";
        int result = CorrectReading.nextInt(kb, msg, 1, 2);    
        return result;
    }
    
 
}
