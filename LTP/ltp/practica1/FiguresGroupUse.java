package practica1;


/**
 * class FiguresGroupUse.
 * 
 * @author LTP 
 * @version 2020-21
 */
public class FiguresGroupUse {
    public static void main(String[] args) {
        FiguresGroup g = new FiguresGroup();
        g.add(new Circle(10, 5, 3.5));
        g.add(new Triangle(10, 5, 6.5, 32));
        System.out.println(g);
        
        FiguresGroup g2 = new FiguresGroup();
        g2.add(new Circle(10, 5, 3.5));
        g2.add(new Triangle(10, 5, 6.5, 32));
        System.out.println("g equals g2: " + g.equals(g2));
    }
}
