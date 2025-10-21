import librerias.estructurasDeDatos.modelos.Stack;

public class LinkedStack<E> implements Stack<E> {
    LinkedNode<E> top;
    int size;

    public LinkedStack() {
        top = null;
        size = 0;
    }

    public void push(E el) {
        top = new LinkedNode<E>(el, top);
        size++;
    }

    public E pop() {
        E el = top.data;
        top = top.next;
        size--;
        return el;
    }

    public E peek() {
        return top.data;
    }

    public boolean isEmpty() {
        return size == 0;
    }
}
