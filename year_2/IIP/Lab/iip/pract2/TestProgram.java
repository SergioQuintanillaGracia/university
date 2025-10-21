package pract2;
 
/**
 * Program class to test all the defined shapes
 * Can be executed from BlueJ <B>workbench</B> 
 * @author IIP
 * @version Academic year 2023/24
 */
public class TestProgram {
    /** No objects of this class are created. */
    private TestProgram() { }

    public static void main(String[] args) {
        // Init blackboard given name and dimensions
        Blackboard bb = new Blackboard("WINDOW TO THE WORLD", 500, 300);
                
        // Yellow circle
        Circle c = new Circle(50, "yellow", 100, 100);
        // put it on the drawing space
        bb.add(c);
        // show on the screen the perimeter of the circle
        System.out.println("The perimeter of the circle is " + c.perimeter());
        
        // Long and red rectangle
        Rectangle r = new Rectangle(100, 10, "red", 50, 155);
        // put it on the drawing space
        bb.add(r);
        
        // Black isosceles triangle
        TrIsosceles t = new TrIsosceles(30, 60, "black", 210, 60 );
        // put it on the drawing space
        bb.add(t);
    }

} // of TestProgram

