public class TestQueueIntLinked {

    public static void main(String[] args) {
        testSingleElementQueue();
        testTwoElementQueue();
        testMultipleElementsQueue();
        testAlreadySortedQueue();
        testReverseSortedQueue();
    }

    public static void testSingleElementQueue() {
        QueueIntLinked queue = new QueueIntLinked();
        queue.enqueue(5);

        System.out.println("Before sorting: " + queue);
        queue.mergeSort();
        System.out.println("After sorting: " + queue);
    }

    public static void testTwoElementQueue() {
        QueueIntLinked queue = new QueueIntLinked();
        queue.enqueue(3);
        queue.enqueue(1);

        System.out.println("Before sorting: " + queue);
        queue.mergeSort();
        System.out.println("After sorting: " + queue);
    }

    public static void testMultipleElementsQueue() {
        QueueIntLinked queue = new QueueIntLinked();
        queue.enqueue(4);
        queue.enqueue(2);
        queue.enqueue(5);
        queue.enqueue(1);
        queue.enqueue(3);

        System.out.println("Before sorting: " + queue);
        queue.mergeSort();
        System.out.println("After sorting: " + queue);
    }

    public static void testAlreadySortedQueue() {
        QueueIntLinked queue = new QueueIntLinked();
        queue.enqueue(1);
        queue.enqueue(2);
        queue.enqueue(3);
        queue.enqueue(4);
        queue.enqueue(5);

        System.out.println("Before sorting: " + queue);
        queue.mergeSort();
        System.out.println("After sorting: " + queue);
    }

    public static void testReverseSortedQueue() {
        QueueIntLinked queue = new QueueIntLinked();
        queue.enqueue(5);
        queue.enqueue(4);
        queue.enqueue(3);
        queue.enqueue(2);
        queue.enqueue(1);

        System.out.println("Before sorting: " + queue);
        queue.mergeSort();
        System.out.println("After sorting: " + queue);
    }
}
