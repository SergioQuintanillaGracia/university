package pract2;
 
/**
 * Program class to test all the defined shapes
 * Can be executed from BlueJ <B>workbench</B> 
 * @author IIP
 * @version Academic year 2023/24
 */
public class MyTest {
    /** No objects of this class are created. */
    private MyTest() { }

    public static void main(String[] args) {
        Blackboard bb = new Blackboard("WINDOW TO THE WORLD", 200, 300);
        
        // Backpack
        Rectangle backpackR = new Rectangle(50, 40, "red", 60, 130);
        bb.add(backpackR);
        
        Circle backpackUpC = new Circle(20, "red", 55, 107);
        bb.add(backpackUpC);
        
        Circle backpackDownC = new Circle(20, "red", 55, 148);
        bb.add(backpackDownC);
        
        // Head
        Circle headC = new Circle(50, "red", 100, 100);
        bb.add(headC);
        
        // Body
        Rectangle bodyR = new Rectangle(100, 95, "red", 100, 140);
        bb.add(bodyR);
        
        // Legs
        Rectangle leg1R = new Rectangle(30, 50, "red", 65, 195);
        bb.add(leg1R);
        
        Rectangle leg2R = new Rectangle(30, 50, "red", 135, 195);
        bb.add(leg2R);
        
        // Visor
        Rectangle visorR = new Rectangle(50, 30, "gray", 115, 95);
        bb.add(visorR);
        
        Circle visorLeftC = new Circle(15, "gray", 90, 95);
        bb.add(visorLeftC);
        
        Circle visorRightC = new Circle(15, "gray", 142, 95);
        bb.add(visorRightC);
    }

} // of TestProgram

