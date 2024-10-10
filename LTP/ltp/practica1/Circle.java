package practica1;


/**
 * class Circle.
 * 
 * @author LTP 
 * @version 2020-21
 */

public class Circle extends Figure {
    private double radius;

    public Circle(double x, double y, double r) {
        super(x, y);
        radius = r;
    }

    public String toString() {
        return "Circle:\n\t" +
            super.toString() +
            "\n\tRadius: " + radius;
    }
    
    @Override
    public boolean equals(Object o) {
        if (!(o instanceof Circle)) {
            return false;
        }
        
        Circle c = (Circle) o;
        
        return super.equals(c) && radius == c.radius;
    }
    
    @Override
    public double area() {
        return Math.PI * radius * radius;
    }
}
