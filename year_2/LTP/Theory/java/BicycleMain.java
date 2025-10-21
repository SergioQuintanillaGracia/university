public class BicycleMain {
    public static void main(String[] args) {
        Bicycle b;
        MountainBike m = new MountainBike(75, 90, 25, 8);
        b = m;
        System.out.println(b);  // Calls the toString method of MountainBike
    }
}
