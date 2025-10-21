public class dice {
    public static void main(String[] args) throws InterruptedException {
        int randomNum;

        while (true) {
            randomNum = (int) (Math.random() * 6 + 1);
            System.out.println("The dice says %d".formatted(randomNum));
            Thread.sleep(10);
        }
    }
}
