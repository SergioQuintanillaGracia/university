public class Bicycle {
    protected int cadence;
    protected int gear;
    protected int speed;

    public Bicycle(int startCad, int startSpeed, int startGear) {
        cadence = startCad;
        speed = startSpeed;
        gear = startGear;
    }

    public void setCadence(int newCad) {
        cadence = newCad;
    }

    public void setGear(int newGear) {
        gear = newGear;
    }
    
    public void applyBrake(int decrement) {
        speed -= decrement;
    }
    
    public void speedUp(int increment) {
        speed += increment;
    }
}