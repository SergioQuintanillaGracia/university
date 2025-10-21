public class Rectangle extends Figure {
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
