public interface ComparableRange<T extends Rectangle> extends Comparable<Figure> {
    int compareToRange(T o);
}