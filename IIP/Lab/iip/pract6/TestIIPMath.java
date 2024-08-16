package pract6;

import graph2D.Graph2D;
import java.awt.Color;

/**
 * Write a description of class TestIIPMath here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class TestIIPMath {
    /* Get the graph of the function IIPMath.sin(x) on the interval [-20, 20]. */
    public static void graph() {
        // Define the range of values for x and for y
        double xMin = -20;
        double xMax = 20;
        double yMin = -2;
        double yMax = 2;
        
        // Create drawing space with desired dimensions.
        Graph2D gd = new Graph2D(xMin, xMax, yMin, yMax, 800, 250);
        gd.setTitle("IIPMath");
        
        // Calculate the increment at each step of x (delta).
        double delta = (xMax - xMin) / Graph2D.INI_WIDTH;
        
        // Traverse each point in x, calculate f(x) and draw (x, f(x)).
        for (double x = xMin; x <= xMax; x += delta) {
            double y = IIPMath.sin(x);
            gd.drawPoint(x, y, Color.BLUE, 2);  
        }
    }
}
