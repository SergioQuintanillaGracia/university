public class LinkedNode<E> {

    private E data;
    LinkedNode<E> next;

    public LinkedNode(E data) {
        this(data, null);
    }

    public LinkedNode(E data, LinkedNode<E> next) {
        set(data);
        this.next = next;
    }

    public E get() {
        return data;
    }

    public void set(E data) {
        this.data = data;
    }
}
