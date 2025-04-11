package libraries.dataStructures.models;

/**
 * Queue Model: FIFO access to the elements of a collection
 * @param <E> data type of the elements in the queue
 *
 * @author (EDA)
 * @version (Curso 2018-2019)
 */

public interface Queue<E> {

    // Modifier Methods -- State of a Queue:
    /** Adds the element e to a Queue, i.e., places it at the end. */
    void enqueue(E e);

    /** IFF !isEmpty():
     *  gets and removes from a Queue its first Element */
    E dequeue();

    // Getter Methods -- State of a Queue
    /** IFF !isEmpty(): gets the Element that occupies the first
      * position of a Queue, the first in insertion order. */
    E first();

    /** Checks whether a Queue is empty. */
    boolean isEmpty();
}
