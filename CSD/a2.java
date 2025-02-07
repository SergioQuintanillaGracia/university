public class a2 extends Thread {
    private int level;
    public a2(int n){
        level = n;
    }

    public void createThread(int i) {
        a2 h = new a2(i);
        if (i>=1)
            h.run();
        System.out.println("Thread of level "+i+" created.");
    }

    public void run() {
        if (level>0)
            createThread(level-1);
        System.out.println("End of thread. Level:" + level);
    }

    public static void main(String[] argv) {
        for (int i=1; i<3; i++)
            new a2(2).start();
    }
}