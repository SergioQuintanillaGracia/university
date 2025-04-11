package libraries.dataStructures.hierarchical;

import libraries.dataStructures.models.PriorityQueue;

/** BinaryHeap class: represents a Heap with an array,
 *  and stores its Root at index 1.
 *
 *  Its main features are:
 *  @param <E>, the type of its elements, must implement Comparable
 *  ATTRIBUTES (protected so that child classes can see them):
 *      HAS AN array to store the elements (E[] theArray)
 *      HAS A size that stores the number of elements contained
 *
 * @author (profesores EDA)
 * @version (Curso 2021-2022)
 **/

public class BinaryHeap<E extends Comparable<E>>
    implements PriorityQueue<E> {

    protected static final int DEFAULT_CAPACITY = 11;

    // A Heap is a Complete Binary Tree, and thus,...
    // HAS AN Implicit Representation
    protected E[] theArray;
    // HAS A size or number of nodes
    protected int size;

    /** Creates an empty Priority Queue (PQ)
     *  with initial capacity DEFAULT_CAPACITY
     */
    public BinaryHeap() { this(DEFAULT_CAPACITY); }

    /** Creates an empty Priority Queue (PQ)
     * with initial capacity n
     */
    @SuppressWarnings("unchecked")
    public BinaryHeap(int n) {
        theArray = (E[]) new Comparable[n];
        size = 0;
    }

    /** Checks whether a PQ is empty */
    public boolean isEmpty() { return size == 0; }

    /** IFF !isEmpty(): gets the datum with the maximum priority in the PQ
     *  @return Element with the highest priority in the PQ
     */
    public E getMin() { return theArray[1]; }

    /** Adds the element e to a PQ, taking into account its priority
     *  @param e Element to be added
     */
    public void add(E e) {
        if (size == theArray.length - 1) { duplicateArray(); }
        int  addPos = ++size;
        while (addPos > 1 && e.compareTo(theArray[addPos / 2]) < 0) {
            theArray[addPos] = theArray[addPos / 2];
            addPos = addPos / 2;
        }
        theArray[addPos] = e;
    }

    // Duplicates the capacity of theArray
    @SuppressWarnings("unchecked")
    protected void duplicateArray() {
        E[] newArray = (E[]) new Comparable[theArray.length * 2];
        System.arraycopy(theArray, 1, newArray, 1, size);
        theArray = newArray;
    }

    /** IFF !isEmpty(): gets and removes the datum with
     *  the highest priority from the PQ
     *  @return Element with maximum priority of the PQ
     */
    public E removeMin() {
        E theMinimum = theArray[1];
        theArray[1] = theArray[size--];
        sink(1);
        return theMinimum;
    }

    //  If necessary, restores the order property of a Heap
    //  by sinking the element of theArray placed
    //  in position pos
    //  @param pos  Position of the element to be sunk
    protected void sink(int pos) {
        int currPos = pos;
        E toSink = theArray[currPos];
        int child = currPos * 2;
        boolean isHeap = false;
        while (child <= size && !isHeap) {
            if (child != size
                && theArray[child + 1].compareTo(theArray[child]) < 0) {
                child++;
            }
            if (theArray[child].compareTo(toSink) < 0) {
                theArray[currPos] = theArray[child];
                currPos = child;
                child = currPos * 2;
            } else { isHeap = true; }
        }
        theArray[currPos] = toSink;
    }
}
