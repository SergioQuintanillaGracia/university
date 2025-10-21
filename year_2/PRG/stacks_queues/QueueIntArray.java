public class QueueIntArray {
    private int arr[];
    private int first, last, size;
    private static final int MAX = 100000;

    public QueueIntArray() {
        arr = new int[MAX];
        size = 0;
        first = 0;
        last = -1;
    }

    public void enqueue(int x) {
        if (size == MAX) {
            throw new IllegalStateException("Queue is full");
        }

        last = (last + 1) % MAX;
        arr[last] = x;
        size++;
    }

    public int dequeue() {
        if (size == 0) {
            throw new IllegalStateException("Queue is empty");
        }

        int x = arr[first];
        first = (first + 1) % MAX;
        size--;
        return x;
    }

    public int first() {
        return arr[first];
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public int size() {
        return size;
    }
}