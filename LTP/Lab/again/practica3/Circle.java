
/**
 * class Circle.
 * 
 * @author LTP 
 * @version 2020-21
 */

public class Circle extends Figure implements Printable {
    private double radius;

    public Circle(double x, double y, double r) {
        super(x, y);
        radius = r;
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof Circle)) return false;

        Circle c = (Circle) o;

        return super.equals(o) && radius == c.radius;
    }

    @Override
    public void print(char c) {
        int n = (int)radius;
        for (int j = 0; j <= n * 2; j++) {
            for (int i = 0; i <= n * 2; i++) {
                if (Math.pow(i - n, 2.0) + Math.pow(j - n, 2.0)
                    <= (int)Math.pow(n, 2)) {
                    System.out.print(c);
                }
                else {
                    System.out.print(" ");
                }
            }
            System.out.println();
        }
    }

    public String toString() {
        return "Circle:\n\t" +
            super.toString() +
            "\n\tRadius: " + radius;
    }

    @Override
    public double area() {
        return 2 * Math.PI * radius;
    }
}