package libraries.dataStructures.linear;

import libraries.dataStructures.models.Queue;

/** Implements the Queue interface with an array
 * @author (profesores EDA )
 * @version 2017
 * @param <E> type of the elements in the queue
 */
public class ArrayQueue<E> implements Queue<E> {
    // Class Attributes
    protected static final int DEFAULT_CAPACITY = 50;
    // Instance Attributes
    protected E[] theArray;
    protected int end, first, size;

    // Constructor
    @SuppressWarnings("unchecked")
    public ArrayQueue() {
        theArray = (E[]) new Object[DEFAULT_CAPACITY];
        size = 0; first = 0; end = -1;
    }

    /* adds x to the end of the Queue */
    public void enqueue(E x) {
        if (size == theArray.length) { duplicateQueue(); }
        end = increment(end);
        theArray[end] = x;
        size++;
    }

    /* IFF !isEmpty(): removes the first element of the Queue */
    public E dequeue() {
        E theFirst = theArray[first];
        first = increment(first);
        size--;
        return theFirst;
    }

    /* IFF !isEmpty(): returns the first element of the Queue */
    public E first() {
        return theArray[first];
    }

    /* checks whether the Queue is empty */
    public boolean isEmpty() {
        return (size == 0);
    }

    /* increments an index in a circular array */
    private int increment(int indice) {
        return (indice + 1) % theArray.length;
    }

    /**
     * duplicates the capacity of the array
     */
    @SuppressWarnings("unchecked")
    private void duplicateQueue() {
        E[] newArray = (E[]) new Object[theArray.length * 2];
        for (int i = 0; i < size; i++) {
            newArray[i] = theArray[first];
            first = increment(first);
        }
        theArray = newArray;
        first = 0;
        end = size - 1;
    }
}
