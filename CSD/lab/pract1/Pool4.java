// CSD feb 2013 Juansa Sendra

public class Pool4 extends Pool { //kids cannot enter if there are instructors waiting to exit

    private int kidsPerInstructor;
    private int cap;
    private int kidsSwimming = 0;
    private int instructorsSwimming = 0;
    private int instructorsWaitingToRest = 0;

    public void init(int ki, int cap) {
        kidsPerInstructor = ki;
        this.cap = cap;
    }

    public synchronized void kidSwims() throws InterruptedException {
        while (
            kidsSwimming >= instructorsSwimming * kidsPerInstructor ||
            kidsSwimming + instructorsSwimming >= cap ||
            instructorsWaitingToRest > 0
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
        instructorsWaitingToRest++;

        while (kidsSwimming > (instructorsSwimming - 1) * kidsPerInstructor) {
            log.waitingToRest();
            wait();
        }

        instructorsSwimming--;
        instructorsWaitingToRest--;

        notifyAll();

        log.resting();
    }
}
