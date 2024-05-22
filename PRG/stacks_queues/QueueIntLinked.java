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

    @Override
    public String toString() {
        StringBuffer sb = new StringBuffer();
        NodeInt curr = this.first;

        sb.append("{");
        
        while (curr != null) {
            sb.append(curr.data);
            
            if (curr.next != null) {
                sb.append(", ");
            }
            
            curr = curr.next;
        }

        sb.append("}");

        return sb.toString();
    }

    public QueueIntLinked splitQueue() {
        QueueIntLinked secondHalf = new QueueIntLinked();
        
        NodeInt prev = null;
        NodeInt curr = this.first;

        for (int i = 0; i < this.size() / 2; i++) {
            prev = curr;
            curr = curr.next;
        }

        secondHalf.first = curr;
        secondHalf.last = this.last;
        this.last = prev;
        prev.next = null;
        
        secondHalf.size = this.size / 2 + this.size % 2;
        this.size = this.size / 2;

        return secondHalf;
    }

    public static QueueIntLinked naturalMerge(QueueIntLinked a, QueueIntLinked b) {
        QueueIntLinked merged = new QueueIntLinked();

        while (!a.isEmpty() && !b.isEmpty()) {
            merged.enqueue(a.first() < b.first() ? a.dequeue() : b.dequeue());
        }

        while (!a.isEmpty()) merged.enqueue(a.dequeue());
        while (!b.isEmpty()) merged.enqueue(b.dequeue());

        return merged;
    }

    public void mergeSort() {
        if (this.size > 1) {
            QueueIntLinked secondHalf;
            
            secondHalf = this.splitQueue();

            this.mergeSort();
            secondHalf.mergeSort();

            QueueIntLinked merged = QueueIntLinked.naturalMerge(this, secondHalf);
            this.first = merged.first;
            this.last = merged.last;
            this.size = merged.size;
        }
    }
}
