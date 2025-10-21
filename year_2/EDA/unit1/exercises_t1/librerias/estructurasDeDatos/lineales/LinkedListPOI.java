package librerias.estructurasDeDatos.lineales;

import librerias.estructurasDeDatos.modelos.ListPOI;

/**
 * Implements the ListPI interface using a doubly linked list ...
 * (a) With a fictitious header node.
 * (b) A reference to the first node.
 * (c) A reference to the last node.
 * (d) To represent the Point of Interest, a reference to the node
 * prior to the one that occupies the point of interest.
 * (e) An int size that represents the size of the list.

 * @version February 2019
 * @param <E> type of data in the structure
 */

public class LinkedListPOI<E> implements ListPOI<E> {

    protected LinkedNode<E> first, prev, last;
    protected int size;

    /** constructs an empty ListPI **/
    public LinkedListPOI() {
        first = last = prev = new LinkedNode<E>(null);
        size = 0;
    }

    /**
     * inserts e before the element that occupies its POI,
     * which remains unchanged.
     *
     * @param e Element to be inserted.
     **/
    public void insert(E e) {
        LinkedNode<E> nuevo = new LinkedNode<E>(e, prev.next);
        prev.next = nuevo;
        if (nuevo.next == null) last = nuevo;
        prev = prev.next;
        size++;
    }

    /**
     * SII !isEnd(): removes from a List the element that occupies
     * its POI which remains unchanged.
     **/
    public void remove() {
        size--;
        if (prev.next == last) last = prev;
        prev.next = prev.next.next;
    }

    /**
     * positions the POI of a List at its beginning, on its first
     * element if not empty.
     **/
    public void begin() {
        prev = first;
    }

    /**
     * SII !isEnd(): positions on the next Element the POI
     * of a List.
     **/
    public void next() {
        prev = prev.next;
    }

    /**
     * positions the POI of a List at its end, behind its last
     * Element if not empty.
     **/
    public void end() {
        prev = last;
    }

    /**
     * SII !isEnd(): gets the element that occupies the POI of
     * a List.

     * @return E, the element that occupies the POI of a List.
     */
    public E get() {
        return prev.next.data;
    }

    /**
     * checks if the POI of a List is at its end.
     *
     * @return true if the POI of a List is at its end and
     *         false otherwise
     **/
    public boolean isEnd() {
        return prev == last;
    }

    /**
     * checks if a ListPI is empty
     *
     * @return true if a ListPI is empty and
     *         false otherwise
     **/
    public boolean isEmpty() {
        return first == last;
    }

    /**
     * returns the size of a ListPI, i.e. its
     * number of elements.

     * @return int, the number of Elements in a ListPI.
     **/
    public int size() {
        return size;
    }

    /**
     * returns the String with the elements of a ListPI
     * in order of insertion.

     * @return String that contains the elements of a ListPI,
     *         in the same format as Java uses for
     *         arrays.
     **/
    public String toString() {
        StringBuilder s = new StringBuilder();
        s.append("[");
        LinkedNode<E> aux = first.next;
        for (int i = 1; i < size; i++, aux = aux.next) {
            s.append(aux.data.toString() + ", ");
        }
        if (size != 0) {
            s.append(aux.data.toString() + "]");
        } else {
            s.append("]");
        }
        return s.toString();
    }
}
