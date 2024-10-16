package practica3;


/**
 * class Figure.
 * 
 * @author LTP 
 * @version 2020-21
 */

public abstract class Figure implements Comparable<Figure> {
    private double x;
    private double y;
    
    public Figure(double x, double y) {
        this.x = x; 
        this.y = y; 
    }
    
    public boolean equals(Object o) {
        if (!(o instanceof Figure)) {
            return false;
        }
        Figure f = (Figure) o;
        return x == f.x && y == f.y;
    }
    
    public String toString() {
        return "Position: (" + x + ", " + y + ") | Area: " + area(); 
    }
    
    public abstract double area();
    
    public int compareTo(Figure f) {
        double diff = area() - f.area();
        
        if (diff > 0) return 1;
        else if (diff < 0) return -1;
        return 0;
    }
}
