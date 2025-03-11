package librerias.estructurasDeDatos.lineales;

import java.util.ArrayDeque;
import librerias.estructurasDeDatos.modelos.*;

public class ArrayDequeQueue<E> extends ArrayDeque<E> implements Queue<E> {

    ArrayDeque arr;

    public ArrayDequeQueue() {
        arr = new ArrayDeque();
    }

    public void enqueue(E e) {
        arr.add(e);
    }

    public E dequeue() {
        return (E) arr.poll();
    }

    public E first() {
        return (E) arr.peekFirst();
    }

    public boolean isEmpty() {
        return arr.size() == 0;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        E[] a = (E[]) arr.toArray();

        sb.append("[");
        for (int i = 0; i < arr.size(); i++) {
            sb.append(a[i]);
            if (i != arr.size() - 1) sb.append(", ");
        }
        sb.append("]");

        return sb.toString();
    }
}
