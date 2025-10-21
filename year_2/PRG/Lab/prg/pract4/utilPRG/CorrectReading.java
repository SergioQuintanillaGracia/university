package pract4.utilPRG;

import java.util.InputMismatchException;
import java.util.Locale;
import java.util.Scanner;
/**
 * Class CorrectReading. Utility class to perform validated reading of
 * from standard input.
 * 
 * @author PRG - ETSINF - UPV
 * @version Academic year 2023/24
 */
public class CorrectReading {
    
    /** There are not objects of this class. */
    private CorrectReading() { }
    
    // ACTIVITY 2:
    /**
     * Reads from a Scanner object and returns an integer value. 
     * @param keyboard Scanner object for reading from.
     * @param message String for asking to the user for the value.
     * @return int, the integer value user entered.
     */
    public static int nextInt(Scanner keyboard, String message) {
        int value = 0;
        boolean someError = true; 
        do {
            try {
                System.out.print(message);
                value = keyboard.nextInt();
                someError = false;
            } catch (InputMismatchException e) {
                System.out.println("Please, type a correct integer! ...");
            } finally {
                keyboard.nextLine();
            }
        } while (someError);
        return value;
    }  
    
    /**
     * Reads from a Scanner object and returns a double value. 
     * @param keyboard Scanner object for reading from.
     * @param message String for asking to the user for the value.
     * @return double, the double value user entered.
     */
    public static double nextDouble(Scanner keyboard, String message) {
        double value = 0;
        boolean someError = true; 
        do {
            try {
                System.out.print(message);
                value = keyboard.nextDouble();
                someError = false;
            } catch (InputMismatchException e) {
                System.out.println("Please, type a correct floating point value! ...");
            } finally {
                keyboard.nextLine();
            }
        } while (someError);
        return value;
    } 
    
    // ACTIVITY 3:
    /**
     * Reads from a Scanner object and returns a double value 
     * checking that it is > 0.
     * 
     * If the value read is a negative real number, it displays 
     *     the message: "Please type a correct positive floating point value!! ... ");
     * If the value is not a real number, the method will display
     *     the message: "Please type a correct floating point number! ... " 
     * 
     * The reading is repeated until it is correct, returning the 
     * first one  that is >= 0.0.
     * 
     * @param keyboard Scanner object for reading from.
     * @param message String for asking the user for the value.
     * @return double, a non-negative double value.
     */    
    public static double nextDoublePositive(Scanner keyboard, String message) {
        double value = 0.0;
        boolean someError = true; 
        do {
            try {
                System.out.print(message);
                value = keyboard.nextDouble();    
                if (value < 0.0) {
                     System.out.println("Please, type a correct positive floating point value!! ... ");
                } else {
                    someError = false;
                }
            } catch (InputMismatchException e) {
                System.out.println("Please type a correct floating point number! ...");
            } finally {
                keyboard.nextLine();
            }
        } while (someError);    
        return value;
    }
    
    // ACTIVITY 4:
    /**
     * Reads from a Scanner object and returns an integer value 
     * in the range <code>[ lowerBound .. upperBound ]</code> 
     * where <code>Integer.MIN_VALUE <= lowerBound</code> and 
     * <code>upperBound <= Integer.MAX_VALUE</code>.
     * 
     * 
     * - If the integer read is out of range, the method throws 
     * an exception of type IllegalArgumentException with 
     * the message: "v is not in the range [lowerBound .. upperBound]" 
     * where v is the read value, and lowerBoud, and upperBound are
     * the given parameters.
     * 
     * The method then catches the exception and displays the 
     * exception message along with the text: 
     *     ". Please, type a correct value inside the bounds!... " 
     *     
     * - If the value is not an integer, the method displays 
     * the message: "Please, type an integer! ... " 
     * 
     * The reading is repeated until the token read is correct 
     * and returns the first one that is a valid integer.
     * 
     * 
     * @param keyboard Scanner object for reading from.
     * @param message String for asking to the user for the value.
     * @param lowerBound int lower bound of the value to be read and accepted.
     * @param upperBound int upper bound of the value to be read and accepted.
     * @return int, the integer number entered by the user.
     */    
    public static int nextInt(Scanner keyboard, String message, 
                              int lowerBound, int upperBound) {
        int value = 0;        
        boolean someError = true;
        do {
            try {
                System.out.print(message);
                value = keyboard.nextInt();
                if (value < lowerBound || value > upperBound) {
                    throw new IllegalArgumentException("v is not in the range [" + lowerBound + " .. " + upperBound + "]");
                }
                someError = false;
            } catch (InputMismatchException e) {
                System.out.println("Please, type an integer! ...");
            } catch (IllegalArgumentException e) {
                System.out.println(e.getMessage() + ". Please, type a correct value inside the bounds!...");
            } finally {
                keyboard.nextLine();
            }
            
        } while (someError);
        return value;
    }  
}
