package practica3;


/**
 * Write a description of class Rectangle here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Rectangle extends Figure implements ComparableRange<Rectangle> {
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
    
    @Override
    public int compareToRange(Rectangle f) {
        double area1 = area();
        double area2 = f.area();
        double diff = Math.abs((area1 - area2) / (area1 + area2));
        
        if (diff <= 0.1) {
            return 0;
        } else {
            return compareTo(f);
        }
    }
}
