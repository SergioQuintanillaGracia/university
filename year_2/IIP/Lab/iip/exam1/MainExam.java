package exam1;

import java.util.Scanner;
import others.TimeInstant;
/**
 * MainExam class: program class that uses the TimeInstant class from the "others" package, 
 * identical to your TimeInstant class with the correct divideBy2() method.  
 * 
 * @author IIP
 * @version Academic Year 2023-24
 */
public class MainExam {
    
    /** No objects of this class are created. */
    private MainExam() { }
    
    public static void main(String[] args) {
        Scanner kbd = new Scanner(System.in);
        System.out.println("Reading hour from keyboard.");
        System.out.print("   -> Input the hour (between 0 and 23): ");
        int h = kbd.nextInt();
        System.out.print("   -> Input the minutes (between 0 and 59): ");
        int m = kbd.nextInt();
        // Once both data have been read from keyboard and assuming they are correct:
        // a) Create a TimeInstant ti with these data. 
        TimeInstant ti = new TimeInstant(h, m);
        
        // b) Show the TimeInstant ti on Screen (with format "hh:mm")
        // toString() is not needed here, as it's automatically executed.
        // However, as the exam is telling us we must use it, I'll use it.
        System.out.println(ti.toString());
        
        // c) Update the hours and minutes of ti dividing by 2,
        //      using the divideBy2 method
        ti.divideBy2();
        
        // d) Show the TimeInstant ti on Screen again (with format "hh:mm")        
        System.out.println(ti.toString());
        
    }
}