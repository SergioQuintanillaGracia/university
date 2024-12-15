package pract3;


/**
 * Write a description of interface ComparableRange here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public interface ComparableRange<T> extends Comparable<Figure> {
    int compareToRange(T o);
}
