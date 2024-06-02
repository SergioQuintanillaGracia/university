public class Test {
    public static void main(String[] args) {
        QueueIntLinked q = new QueueIntLinked();
        q.enqueue(4);
        q.enqueue(6);
        q.enqueue(8);
        q.enqueue(10);
        q.enqueue(5);
        q.enqueue(7);
        System.out.println("Original queue: " + q);

        q.swapThirds();
        System.out.println("Sqapped queue: " + q);
    }
}
