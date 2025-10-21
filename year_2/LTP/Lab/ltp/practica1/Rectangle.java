package practica1;


/**
 * Write a description of class Rectangle here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Rectangle extends Figure {
    private double side1;
    private double side2;
    
    public Rectangle(double x, double y, double side1, double side2) {
        super(x, y);
        this.side1 = side1;
        this.side2 = side2;
    }
    
    public String toString() {
        return "Rectangle:\n\t" +
            super.toString() +
            "\n\tSide1: " + side1 +
            "\n\tSide2: " + side2;
    }
    
    @Override
    public boolean equals(Object o) {
        if (!(o instanceof Rectangle)) {
            return false;
        }
        
        Rectangle r = (Rectangle) o;
        
        return super.equals(r) && side1 == r.side1 && side2 == r.side2;
    }
    
    @Override
    public double area() {
        return side1 * side2;
    }
}
