// CSD feb 2015 Juansa Sendra

public class Pool3 extends Pool { //max capacity

    private int kidsPerInstructor;
    private int cap;
    private int kidsSwimming = 0;
    private int instructorsSwimming = 0;

    public void init(int ki, int cap) {
        kidsPerInstructor = ki;
        this.cap = cap;
    }

    public synchronized void kidSwims() throws InterruptedException {
        while (
            kidsSwimming >= instructorsSwimming * kidsPerInstructor ||
            kidsSwimming + instructorsSwimming >= cap
        ) {
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

    public synchronized void instructorSwims() throws InterruptedException {
        while (kidsSwimming + instructorsSwimming >= cap) {
            log.waitingToSwim();
            wait();
        }

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

        notifyAll();

        log.resting();
    }
}
