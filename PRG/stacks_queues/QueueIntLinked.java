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

    public void naturalMerge_2(QueueIntLinked other) {
        NodeInt a = this.first;
        NodeInt b = other.first;
        NodeInt cf = null;
        NodeInt c = null;

        while (a != null && b != null) {
            if (a.data <= b.data) {
                if (cf == null) {
                    cf = c = a;
                    a = a.next;
                    c.next = null;
                } else {
                    c.next = a;
                    a = a.next;
                    c = c.next;
                    c.next = null;
                }
            } else {
                if (cf == null) {
                    cf = c = b;
                    b = b.next;
                    c.next = null;
                } else {
                    c.next = b;
                    b = b.next;
                    c = c.next;
                    c.next = null;
                }
            }
        }

        if (a != null) {
            c.next = a;
        
        } else {
            c.next = b;
            this.last = other.last; // This is maybe not needed
        }

        this.first = cf;
        this.size = this.size + other.size;
        other.size = 0;
        other.first = other.last = null;
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

    public void splitExam(int x) {
        NodeInt curr = this.first;
        NodeInt prev = null;

        while (curr != null) {
            if (curr.data == x) {
                curr.data = x / 2;
                curr.next = new NodeInt(x / 2 + x % 2, curr.next);
                break;
            }

            prev = curr;
            curr = curr.next;
        }

        if (curr == null) {  // Igual esto hay que arreglarlo
            last = prev.next;
        }
    }

    public void splitJonAnder(int x) {
        NodeInt n = this.first;

        while (n != null && n.data != x) {
            n = n.next;
        }

        if (n != null) {
            n.next = new NodeInt(x / 2 + x % 2, n.next);
            n.data = x/2;

            if (n == this.last) {
                this.last = n.next;
            }

            size++;
        }
    }

    public void swapThirds() {
        // Stores the first 3 elements at first, and then the last
        // three ones
        int arr[] = new int[size / 3];
        NodeInt curr = first;
        int i = 0;  // Index of the current element

        while (curr != null) {
            if (i < size / 3) {
                // The value is in the first third of the queue, store it
                // in the array
                arr[i] = curr.data;
            
            } else if (i >= size / 3 * 2) {
                // The value is in the last third of the queue, swap it
                // by the corresponding one stored in the array
                int temp = arr[i - size / 3 * 2];
                arr[i - size / 3 * 2] = curr.data;
                curr.data = temp;
            }

            curr = curr.next;
            i++;
        }

        // Now, the queue contains in its last third the same elements
        // as in its first third, and the array contains the original
        // elements in the last third of the queue
        // We need to replace the first third of the queue by the
        // elements in the array
        curr = first;
        i = 0;

        while (i < size / 3) {
            curr.data = arr[i++];
            curr = curr.next;
        }
    }
}
