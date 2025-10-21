public abstract class Shape {
    private double x, y;
    
    public Shape(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public void move(double distX, double distY) {
        x += distX;
        y += distY;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    public abstract double perimeter();
    public abstract double area();
}
