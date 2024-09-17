public class MountainBike extends Bicycle {
    private int seatHeight;

    public MountainBike(int startHeight, int startCad, int startSpeed,
                        int startGear) {
        super(startCad, startSpeed, startGear);  // Call the superclass
                                                 // constructor
        seatHeight = startHeight;
    }

    public int getSeatHeight() {
        return seatHeight;
    }

    @Override
    public String toString() {
        return ("MountainBike, gear=" + gear + ", cadence=" + cadence
                + ", speed=" + speed + ", height=" + seatHeight);
    }
}