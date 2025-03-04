// CSD Mar 2013 Juansa Sendra

public class LimitedTable extends RegularTable { // max 4 in dinning-room
    static final int LIMIT = 4;
    int insideCount = 0;

    public LimitedTable(StateManager state) {
        super(state);
    }

    @Override
    public synchronized void enter(int id) throws InterruptedException {
        while (insideCount >= LIMIT) {
            wait();
        }
        insideCount++;
    }

    @Override
    public synchronized void exit(int id) {
        insideCount--;
        notifyAll();
    }
}
