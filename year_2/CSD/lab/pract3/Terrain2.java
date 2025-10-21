import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Terrain2 implements Terrain {
    Lock lock = new ReentrantLock();
    Condition cond[][];
    Viewer v;

    public Terrain2(int t, int ants, int movs, String msg) {
        v = new Viewer(t, ants, movs, msg);
        cond = new Condition[t][t];
        for (int i = 0; i < t; i++) {
            for (int j = 0; j < t; j++) {
                cond[i][j] = lock.newCondition();
            }
        }
    }

    public void hi(int a) {
        lock.lock();
        v.hi(a);
        lock.unlock();
    }

    public void bye(int a) {
        lock.lock();
        cond[v.getPos(a).x][v.getPos(a).y].signal();
        v.bye(a);
        lock.unlock();
    }

    public void move(int a) throws InterruptedException {
        lock.lock();
        v.turn(a);
        Pos dest = v.dest(a);
        while (v.occupied(dest)) {
            cond[dest.x][dest.y].await();
            v.retry(a);
        }
        v.go(a);
        cond[v.getPos(a).x][v.getPos(a).y].signal();
        lock.unlock();
    }
}
