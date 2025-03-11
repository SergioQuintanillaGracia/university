import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Terrain1 implements Terrain {
    Lock lock = new ReentrantLock();
    Condition cond;
    Viewer v;

    public Terrain1(int t, int ants, int movs, String msg) {
        v = new Viewer(t, ants, movs, msg);
        cond = lock.newCondition();
    }

    public void hi(int a) {
        lock.lock();
        v.hi(a);
        lock.unlock();
    }

    public void bye(int a) {
        lock.lock();
        cond.signalAll();
        v.bye(a);
        lock.unlock();
    }

    public void move(int a) throws InterruptedException {
        lock.lock();
        v.turn(a);
        Pos dest = v.dest(a);
        while (v.occupied(dest)) {
            cond.await();
            v.retry(a);
        }
        v.go(a);
        cond.signalAll();
        lock.unlock();
    }
}
