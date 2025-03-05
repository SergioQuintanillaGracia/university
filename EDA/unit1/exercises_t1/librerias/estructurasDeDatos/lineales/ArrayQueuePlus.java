package librerias.estructurasDeDatos.lineales;

import librerias.estructurasDeDatos.modelos.*;

public class ArrayQueuePlus<E> extends ArrayQueue<E> implements QueuePlus<E> {
    public final int size() {
        return super.size;
    }

    private int decrement(int i) {
        i--;
        if (i < 0)
            i = arr.length - 1;
        return i;
    }

    public void reverse() {
        int left = begin;
        int right = decrement(end);

        while (left != right && decrement(left) != right) {
            E aux = arr[left];
            arr[left] = arr[right];
            arr[right] = aux;
            left = increment(left);
            right = decrement(right);
        }
    }

    public void reverseMethodsRecursive() {
        if (!isEmpty()) {
            E el = dequeue();
            reverseMethodsRecursive();
            enqueue(el);
        }
    }
}
