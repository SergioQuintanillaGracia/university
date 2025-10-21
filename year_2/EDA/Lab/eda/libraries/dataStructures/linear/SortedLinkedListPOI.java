package libraries.dataStructures.linear;


/**
 * Write a description of class SortedLinkedListPOI here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class SortedLinkedListPOI<T extends Comparable> extends LinkedListPOI<T> {
    @Override
    public void add(T item) {
        begin();
        
        while (!isEnd() && item.compareTo(get()) > 0) {
            next();
        }
        
        super.add(item);
    }
}
