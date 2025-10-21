package pract3;

import java.util.Scanner;
/**
 *  Class Test3.
 *  A first class with data input from keyboard, 
 *  and use of operations with int, long, Math, and String.
 *  It has three compilation errors.
 *  @author IIP 
 *  @version Academic Year 2023-24
 */

public class Test3 {

    public static void main(String[] args){
        Scanner kbd = new Scanner(System.in);
        System.out.println("Reading hour from keyboard.");
        
        System.out.print("   -> Input the hour (between 0 and 23): ");
        int h = kbd.nextInt();
        String hh = h < 10 ? "0" + h : Integer.toString(h);
        
        System.out.print("   -> Input the minutes (between 0 and 59): ");
        int m = kbd.nextInt();
        String mm = m < 10 ? "0" + m : Integer.toString(m);
        
        System.out.println("Input hour: " + hh + ":" + mm);
        
        if (isPalindrome(hh + mm)) {
            System.out.println("Btw, that hour is a palindrome!");
        }
     
        
        long tMinTotal = System.currentTimeMillis() / (60 * 1000);
        int tMinCurrent = (int) (tMinTotal % (24 * 60));
        int tHoursCurrent = tMinCurrent / 60;
        tMinCurrent = tMinCurrent % 60;
        
        System.out.println("Current hour: " + tHoursCurrent + ":" + tMinCurrent);
        
        
        m = m + h * 60;
        tMinCurrent = tMinCurrent + tHoursCurrent * 60;
        int minDifference = m - tMinCurrent < 0 ? tMinCurrent - m : m - tMinCurrent;
        int h2 = minDifference / 60;
        int m2 = minDifference % 60;
        
        System.out.println("Difference in minutes between the two hours: " + minDifference + " (" + h2 + "h and " + m2 + "m)");
    }    
    
    
    static boolean isPalindrome(String str) {
        boolean isPal = true;
            
            for (int i = 0; i < str.length() / 2; i++) {
                if (str.charAt(i) != str.charAt(str.length() - 1 - i)) {
                    isPal = false;
                    break;
                }
            }
        
        return isPal;
    }
}
