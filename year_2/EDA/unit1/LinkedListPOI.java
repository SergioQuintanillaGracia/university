public class LinkedListPOI<E> implements ListPOI<E> {

    LinkedNode<E> first;
    LinkedNode<E> prev; // Points to the node previous to POI
    LinkedNode<E> last;
    int size;

    public LinkedListPOI() {
        size = 0;
        first = prev = last = new LinkedNode<E>(null);
    }

    public void add(E data) {
        LinkedNode<E> newNode = new LinkedNode<E>(data, prev.next);
        prev.next = newNode;
        if (newNode.next == null) last = newNode;
        prev = prev.next;
        size++;
    }

    public void remove() {
        if (prev.next == null) throw new IndexOutOfBoundsException();

        prev.next = prev.next.next;
        if (prev.next == null) last = prev;
        size--;
    }

    public void begin() {
        prev = first;
    }

    public void next() {
        if (prev.next == null) throw new IndexOutOfBoundsException();
        prev = prev.next;
    }

    public void end() {
        prev = last;
    }

    public E get() {
        if (prev.next == null) throw new IndexOutOfBoundsException();
        return prev.next.get();
    }

    public boolean isEnd() {
        return prev == last;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public int size() {
        return size;
    }
}
