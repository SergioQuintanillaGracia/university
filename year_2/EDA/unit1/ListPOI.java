public interface ListPOI<E> {
    // Adds e to a list before the element at its POI
    void add(E e);

    // Removes the element at the POI of the list
    void remove();

    // Moves the POI to the first element of the list
    void begin();

    // Advances by one the list POI
    void next();

    // moves the POI after the last element of the list
    void end();

    // Returns the element at the POI
    E get();

    // Checks whether the POI is at the end of the list
    boolean isEnd();

    // Checks whether the list is empty
    boolean isEmpty();

    // Returns the size of the list
    int size();
}
