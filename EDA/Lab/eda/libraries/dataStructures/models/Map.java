package libraries.dataStructures.models;

/**
 * Dictionary Model WITHOUT repeated keys, or Map: Dynamic Search
 * for a given Key's Entry in a Collection, to recover the
 * Value associated to it.
 * @param <K>, the Keys' type
 * @param <V>, the Values' type
 *
 * @author (EDA)
 * @version (Curso 2018-2019)
 */

public interface Map<K, V> {

    /** Inserts the Entry (k, v) in a Map and returns null; if
     *  the Map contains another Entry with Key k, returns its
     *  associated value and replaces it with v (updates the Entry).
     */
    V put(K k, V v);

    /** removes the Entry with Key k from a Map, and returns its
     *  associated value, or null if no Entry exists with the given key
     */
    V remove(K k);

    /** returns the value associated to a Key k in a Map,
     *  or null if no Entry exists with the given key
     */
    V get(K k);

    /** checks whether a Map is empty */
    boolean isEmpty();

    /** returns the size, or number of Entries of the Map */
    int size();

    /** returns a ListPOI with the size() Keys of a Map */
    ListPOI<K> keys();
}

