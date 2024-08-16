package pract1_2; 
 
// Imports classe Graph2D (from package graph2D).
import graph2D.Graph2D;
// Imports class Color from package java.awt for chaning
// the colours of the elements to be drawn.
import java.awt.Color;
/**
 * Drawing figures of type "Recursive Square".
 * From "Introduction to Programming in Java",
 * Sedgewick & Wayne, Princeton.
 * @author IIP-PRG-DSIC-ETSINF. pmarques
 * @version Academic year 2023-24
 */
public class RSquare {    
    /** There are no objects of this class. */
    private RSquare() { }
    
    /** Draws in the window gd a grey filled square with
     *  the ledge highlighted in blue, centred at ( cX, cY )
     *  and lateral length equal to l.
     */
    public static void drawCentSquare(Graph2D gd, double cX, double cY, double l) {
        // delay();                                           
        // Computes the coordinates of the upper left corner (luX, luY)
        // of the square from its centre (cX, cY) and its lateral length l:
        double luX = cX - l / 2;
        double luY = cY + l / 2;      
        // Draws a grey coloured solid rectangle, with the same vertical
        // and horizontal lateral length equal to l.
        // The upper left corner of the rectangle is at (luX, luY):
        gd.fillRect(luX, luY, l, l, Color.LIGHT_GRAY, 2);
        // Draws a non-filled rectangle with blue border, 
        // with the same vertical and horizontal lateral length equal to l.
        // The upper left corner of the rectangle is at (luX, luY):
        gd.drawRect(luX, luY, l, l, Color.BLUE, 2);
    }
    
    /** Waits 100 ms to continue the execution of the current thread. */
    private static void delay() {
        try { Thread.sleep(100); } catch (Exception e) { ; }
    }
    
    /** Draws in the window gd an RSquare-A figure
     *  of order n >= 1, with the central square centered at
     *  (cX, cY) and lateral length equal to $l$.
     */
    public static void rSquareA(Graph2D gd, int n, double cX, double cY, double l) {
        if (n == 1) {
            drawCentSquare(gd, cX, cY, l);
        } else {
            rSquareA(gd, n - 1, cX - l / 2, cY + l / 2, l / 2);
            rSquareA(gd, n - 1, cX + l / 2, cY + l / 2, l / 2);
            rSquareA(gd, n - 1, cX + l / 2, cY - l / 2, l / 2);
            rSquareA(gd, n - 1, cX - l / 2, cY - l / 2, l / 2);
            drawCentSquare(gd, cX, cY, l);
        }
    } 
    
    /** Draws an RSquare-A figure of order n >= 1
     * with lateral length equal to 1.0 and centered at (0, 0).
     */
    public static void rSquareA(int n) {
        double l = 1.0;
        String title = "RSQuareA: ord. " + n;
        Graph2D gd = new Graph2D(-l, +l, -l, +l);
        gd.setTitle(title);
        rSquareA(gd, n, 0, 0, l);
    }
    
    /** Draws in the window gd an RSquare-B figure
     *  of order n >= 1, with the central square centered at
     *  (cX, cY) and lateral length equal to $l$.
     */
    public static void rSquareB(Graph2D gd, int n, double cX, double cY, double l) {
        if (n == 1) {
            drawCentSquare(gd, cX, cY, l);
        } else {
            drawCentSquare(gd, cX, cY, l);
            rSquareB(gd, n - 1, cX - l / 2, cY + l / 2, l / 2);
            rSquareB(gd, n - 1, cX + l / 2, cY + l / 2, l / 2);
            rSquareB(gd, n - 1, cX + l / 2, cY - l / 2, l / 2);
            rSquareB(gd, n - 1, cX - l / 2, cY - l / 2, l / 2);
        }
    }     
    
    /** Draws an RSquare-B figure of order n >= 1
     * with lateral length equal to 1.0 and centered at (0, 0).
     */
    public static void rSquareB(int n) {
        double l = 1.0;
        String title = "RSQuareB: ord. " + n;
        Graph2D gd = new Graph2D(-l, +l, -l, +l);
        gd.setTitle(title);
        rSquareB(gd, n, 0, 0, l);        
    }
}
