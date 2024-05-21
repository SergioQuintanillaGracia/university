public class QueueIntLinked {
    private NodeInt first, last;
    private int size;

    public QueueIntLinked() {
        first = null;
        last = null;
        size = 0;
    }

    public void enqueue(int x) {
        NodeInt newNode = new NodeInt(x);
        
        if (last != null) {
            last.next = newNode;
        } else {
            first = newNode;
        }

        last = newNode;
        size++;
    }

    public int dequeue() {
        int data = first.data;
        first = first.next;
        
        size--;
        
        if (size == 0) {
            last = null;
        }

        return data;
    }

    public int first() {
        return first.data;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public int size() {
        return size;
    }
}
