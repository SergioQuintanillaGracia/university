// public class a1 extends Thread {
//     protected int n;
//     public a1(int n) {this.n = n;}
//     public void delay(int ms) {
//         // suspends execution for ms milliseconds
//         try {
//             sleep(ms);
//         } catch (InterruptedException ie) {
//             ie.printStackTrace();
//         }
//     }

//     public void run() {
//         for (int i=0; i<10; i++) {
//             System.out.println("Thread "+n +" iteration "+i);
//             delay((n+1)*1000);
//         }

//         System.out.println("End of thread "+n);
//     }

//     public static void main(String[] argv) {
//         System.out.println("--- Begin of execution ---- ");
//         for (int i=0; i<6; i++)
//             new a1(i).start();
//         System.out.println("--- End of execution ---- ");
//     }
// }


class a12 implements Runnable {  // CHANGE TO implements Runnable
    protected int n;
    public a12(int n) {this.n = n;}
    public void delay(int ms) {
        // suspends execution for ms milliseconds
        try {
            Thread.sleep(ms);  // CHANGE TO Thread.sleep(ms)
        } catch (InterruptedException ie) {
            ie.printStackTrace();
        }
    }

    public void run() {
        for (int i=0; i<10; i++) {
            System.out.println("Thread "+n +" iteration "+i);
            delay((n+1)*30);
        }

        System.out.println("End of thread "+n);
    }

    public static void main(String[] argv) {
        System.out.println("--- Begin of execution ---- ");
        Thread last = null;

        for (int i=0; i<6; i++) {
            last = new Thread(new a12(i));  // CHANGE
            last.start();
        }

        try {
            last.join();
        } catch (InterruptedException e) {}

        System.out.println("--- End of execution ---- ");
    }
}