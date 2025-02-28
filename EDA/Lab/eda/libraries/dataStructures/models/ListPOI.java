package libraries.dataStructures.models;

/**
 * A model for a generic List with a Point of Interest (POI),
 * which can be used to access the list's elements.
 *
 * @date February 2019
 * @param <E> type of the List's items
 */
public interface ListPOI<E> {

    /** Adds element e just before the POI, without moving it.
     */
    void add(E e);

    /**
     * IFF !isEnd(): Removes from the list the element the POI
     * is pointing to, without moving the POI's position.
     */
    void remove();

    /** IFF !isEnd(): Recovers the element that the POI is pointing to.
     */
    E get();

    /** Moves the POI to the first element of the list.
     */
    void begin();

    /** IFF !isEnd(): Moves the POI to the next element of the list.
     */
    void next();

    /** Moves the POI after the last element of the list.
     */
    void end();

    /** Checks whether the POI is after the last element.
     */
    boolean isEnd();

    /** Checks whether the List is empty.
     */
    boolean isEmpty();

    /** Returns the number of elements in the List.
     */
    int size();
}
