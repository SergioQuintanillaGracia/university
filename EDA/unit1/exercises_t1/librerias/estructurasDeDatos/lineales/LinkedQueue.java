import java.util.Queue;

public class LinkedQueue<E> implements Queue<E> {
    LinkedNode<E> begin;
    LinkedNode<E> end;
    int size;

    public LinkedQueue() {
        begin = end = null;
        size = 0;
    }

    public void enqueue(E el) {
        LinkedNode<E> newNode = new LinkedNode<>(el);

        if (isEmpty()) {
            begin = newNode;
        } else {
            end.next = newNode;
        }

        end = newNode;
        size++;
    }

    public E dequeue() {
        E el = begin.data;
        begin = begin.next;
        if (begin == null)
            end = null;
        size--;
        return el;
    }

    public E first() {
        return begin.data;
    }

    public boolean isEmpty() {
        return begin == null;
    }
}
