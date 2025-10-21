package libraries.dataStructures.models;

/**
 * Model of a Priority Queue, or Dynamic Search of the
 * Element with the highest priority in a Collection
 *
 * @version Febrero 2018
 * @param <E> type of the elements in the data structure
 * RESTRICTED BY Comparable
 */

public interface PriorityQueue<E extends Comparable<E>> {

    // Modifier Methods of the state of a Priority Queue (PQ)
    /** Taken into account its priority, adds Element e to a PQ. */
    void add(E e);
    /** IFF !isEmpty():
     *  gets and removes the Element with the highest priority from a PQ. */
    E removeMin();

    // Getter Methods of the state of a Priority Queue (PQ)
    /** IFF !isEmpty():
     *  gets the Element with the highest priority from a PQ. */
    E getMin();
    /** Checks whether a PQ is empty. */
    boolean isEmpty();
}
