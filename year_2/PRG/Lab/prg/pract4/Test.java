package pract4;


/**
 * Write a description of class Test here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Test {
    public static void main(String[] args) {
        try {
            throw new NumberFormatException("test");
        } catch (IllegalArgumentException e) {
            System.out.println("caught");
        } finally {
            System.out.println("Finished");
        }
    }
}
