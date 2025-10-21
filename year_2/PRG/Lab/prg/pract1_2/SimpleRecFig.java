package pract1_2;

// Imports class Graph2D from package graph2D.
import graph2D.Graph2D;
// Imports class Color from package java.awt for changing
// the colour of the figures to be drawn.
import java.awt.Color;
/**
 * The class <code>SimpleRecFig</code> contains the
 * basic components for recursively drawing figures.
 * 
 * @author PRG-DSIC-ETSINF, pmarques 
 * @version Academic year 2023-24
 */
public class SimpleRecFig {
    
    /** There are no objects of this class. */
    private SimpleRecFig() { }

    /**
     * Recursively draws a serie of squares in a diagonal line. 
     * The number of squares is given by the parameter
     * <code>iniOrd</code>. 
     * @param iniOrd int, number of squares to be drawn.
     * PRECONDITION: iniOrd >= 1
     */
    public static void recFig(int iniOrd) {
        // Actual ranges of the axes in the drawing canvas
        double minX = -0.1, maxX = 2.1;
        double minY = -0.1, maxY = 2.1;
        
        Graph2D gd = new Graph2D(minX, maxX, minY, maxY);
        // Window title
        gd.setTitle("Recursive Figure: Chained Squares. Order: " + iniOrd); 
        
        // Figure of iniOrd order, with iniOrd squares.
        // The largest square of the figure has lateral length 1.0,
        // and with top left corner at (1.0, 1.0)
        // Initial call to the recursive method
        recFig(gd, iniOrd, 1.0, 1.0, 1.0);
    }
   
    /**
     * Iteratively draws a serie of squares in a diagonal line.
     * The number of squares is given by the parameter
     * <code>iniOrd</code>.
     * @param iniOrd int, number of squares to be drawn.
     * PRECONDITION: iniOrd >= 1
     */
    public static void iterFig(int iniOrd) {
        // Actual ranges of the axes in the drawing canvas
        double minX = -0.1, maxX = 2.1;
        double minY = -0.1, maxY = 2.1;
        
        Graph2D gd = new Graph2D(minX, maxX, minY, maxY);
        // Window title
        gd.setTitle("Iterative figure: Chained Squares. Order: " + iniOrd); 
        
        // Figure of iniOrd order, with iniOrd squares
        // The largest square of the figure has lateral length 1.0,
        // and with top left corner at (1.0, 1.0)
        // Call to the iterative method
        iterFig(gd, iniOrd, 1.0, 1.0, 1.0);
    }
    
    /**
     * Recursively draws a number of ord squares in a diagonal line, 
     * with decreasing sides. The largest square has lateral length l, 
     * and the rest of the squares they reduce their lateral length by half. 
     * @param g Graph2D, the drawing canvas, an object of the class Graph2D.
     * @param ord int, number of squares.
     * @param l double, lateral length of the largest square.
     * @param xSL double, abscissa of the upper left corner of the largest square.
     * @param ySL double, ordinate of the upper left corner of the largest square.
     * PRECONDITION: ord >= 1 
     */
    private static void recFig(Graph2D g, int ord, double l, double xSL, double ySL) {
        // Base case, ord == 1, a square of lateral length l is drawn.
        // Recursive step, ord > 1, a square of lateral length l is drawn, and a 
        // subfigure of ord - 1 squares is drawn.
        if (ord == 1) {
            delay();
            g.drawRect(xSL, ySL, l, l, Color.BLUE, 2);
        } else {
            delay();
            g.drawRect(xSL, ySL, l, l, Color.BLUE, 2);
            recFig(g, ord - 1, l / 2, xSL - l / 2, ySL + l / 2);
        }
    } 

    /**
     * Iteratively draws a number of ord squares in a diagonal line, 
     * with decreasing sides. The largest square has lateral length l, 
     * and the rest of the squares they reduce their lateral length by half.
     * @param g Graph2D, the drawing canvas, an object of the class Graph2D.
     * @param ord int, number of squares.
     * @param l double, lateral length of the largest square.
     * @param xSL double, abscissa of the upper left corner of the largest square.
     * @param ySL double, ordinate of the upper left corner of the largest square.
     * PRECONDITION: ord >= 1 
     */
    private static void iterFig(Graph2D g, int ord, double l, double xSL, double ySL) {
        for (int i = ord; ord >= 1; ord --) {        
            g.drawRect(xSL, ySL, l, l, Color.RED, 2);
            delay();
            xSL = xSL - l / 2;
            ySL = ySL + l / 2;
            l = l / 2;
        }
    }           
    
   /** Waits 300 ms to continue the execution of the current thread. */
    private static void delay() {
        try { Thread.sleep(300); } catch (Exception e) { ; }
    }
}
