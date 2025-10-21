// CSD feb 2015 Juansa Sendra

public class Pool2 extends Pool { //max kids/instructor

    private int kidsPerInstructor;
    private int kidsSwimming = 0;
    private int instructorsSwimming = 0;

    public void init(int ki, int cap) {
        kidsPerInstructor = ki;
    }

    public synchronized void kidSwims() throws InterruptedException {
        while (kidsSwimming >= instructorsSwimming * kidsPerInstructor) {
            log.waitingToSwim();
            wait();
        }

        kidsSwimming++;

        log.swimming();
    }

    public synchronized void kidRests() {
        kidsSwimming--;

        notifyAll();

        log.resting();
    }

    public synchronized void instructorSwims() {
        instructorsSwimming++;

        notifyAll();

        log.swimming();
    }

    public synchronized void instructorRests() throws InterruptedException {
        while (kidsSwimming > (instructorsSwimming - 1) * kidsPerInstructor) {
            log.waitingToRest();
            wait();
        }

        instructorsSwimming--;

        log.resting();
    }
}
