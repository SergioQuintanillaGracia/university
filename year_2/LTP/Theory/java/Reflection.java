import java.lang.reflect.*;

public class Reflection {
    public static void main(String[] args) {
        Circle c = new Circle(4, 2.2, 25.1);
        Class classInfo = c.getClass();
        System.out.println(classInfo);

        Field[] declaredVars = classInfo.getDeclaredFields();
        for (Field f : declaredVars) {
            System.out.printf("Found global variable: %s\n", f.getName());
        }
    }
}