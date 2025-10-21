package pract3;

public class Rectangle extends Figure implements ComparableRange<Rectangle>, Printable {
    double base;
    double height;

    public Rectangle(double x, double y, double b, double h) {
        super(x, y);
        base = b;
        height = h;
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof Rectangle)) return false;

        Rectangle r = (Rectangle) o;

        return super.equals(r) && base == r.base && height == r.height;
    }
    
    @Override
    public void print(char c) {
        int b = (int) base;
        int h = (int) height;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < b; j++) {
                System.out.print(c);
            }
            System.out.println();
        }
    }
    
    @Override
    public int compareToRange(Rectangle r) {
        double a1 = area();
        double a2 = r.area();
        double diff = Math.abs((a1 - a2) / (a1 + a2));

        if (diff <= 0.1) return 0;
        return compareTo(r);
    }

    @Override
    public String toString() {
        return "Rectangle:\n\t" +
            super.toString() +
            "\n\tBase: " + base +
            "\n\tHeight: " + height;
    }

    @Override
    public double area() {
        return base * height;
    }
}
