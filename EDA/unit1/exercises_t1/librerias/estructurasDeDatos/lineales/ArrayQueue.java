package librerias.estructurasDeDatos.lineales;

import librerias.estructurasDeDatos.modelos.*;

public class ArrayQueue<E> implements Queue<E> {
    protected E[] arr;
    protected int endC;
    protected int beginC;
    protected int size;
    protected static final int DEFAULT_CAPACITY = 50;

    /** constructs an empty queue **/
    @SuppressWarnings("unchecked")
    public ArrayQueue() {
        arr = (E[]) new Object[DEFAULT_CAPACITY];
        size = 0;
        beginC = 0;
        endC = 0;
    }

    /** inserts the element e into a Queue, or situates it at its end **/
    // if there is not enough space in arr it doubles
    // the size of the circular Array
    public void enqueue(E e) {
        if (size == arr.length)
            duplicateCircularArray();
        arr[endC] = e;
        endC = increment(endC);
        size++;
    }

    // doubles the current size of a circular Array
    @SuppressWarnings("unchecked")
    protected void duplicateCircularArray() {
        E[] newArr = (E[]) new Object[arr.length * 2];
        for (int i = 0; i < size; i++, beginC = increment(beginC))
            newArr[i] = arr[beginC];
        arr = newArr;
        beginC = 0;
        endC = size;
    }

    // increments an index of a circular Array
    protected int increment(int index) {
        if (++index == arr.length)
            index = 0;
        return index;
    }

    /**
     * SII !isEmpty():
     * obtains and removes from a Queue the Element that occupies its beginning
     **/
    // the beginning is incremented circularly
    public E dequeue() {
        E firstElement = arr[beginC];
        beginC = increment(beginC);
        size--;
        return firstElement;
    }

    /**
     * SII !isEmpty():
     * obtains the Element that occupies the beginning of a Queue,
     * the first in order of insertion
     **/
    public E first() {
        return arr[beginC];
    }

    /** checks if a Queue is empty **/
    public boolean isEmpty() {
        return size == 0;
    }

    /**
     * obtains a String with the Elements of a Queue in FIFO order,
     * or insertion order, and with the format used in the standard Java.
     * For example, if there is a Queue with the Integers from 1 to 4,
     * in FIFO order, toString returns [1, 2, 3, 4];
     * if the Queue is empty, then it returns []
     */
    // NOTE: circularity of arr is considered not only using the increment method
    // but counting the number of Elements from 0 to size-1
    public String toString() {
        // NOTE: use the StringBuilder class instead of String
        // for efficiency reasons
        StringBuilder res = new StringBuilder();
        res.append("[");
        // NOTE: for formatting purposes ...
        // -Iterate over the ArrayQueue
        // from the first to the PENULTIMATE of its elements;
        // -For each element visited,
        // if aux is the variable of the loop iteration,
        // add (append) to res aux.dato + ", "
        int aux = beginC;
        for (int i = 0, j = size - 1; i < j; i++, aux = increment(aux))
            res.append(arr[aux].toString() + ", ");
        // NOTE: for formatting purposes, after the loop ends,
        // add the last element to the result;
        // depending on the size, said element is
        // the String arr[aux].toString()+"]" or the String "]"
        if (size != 0)
            res.append(arr[aux].toString() + "]");
        else
            res.append("]");
        return res.toString();
    }
}
