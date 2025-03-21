package libraries.dataStructures.scattered;

/**
 * Represents an element of the ListPOI that implements
 * a bucket in a HashTable, and thus an Entry or key-value pair.
 *
 * @param <K>, the key's type
 * @param <V>, the value's type
 * 
 * @author (EDA)
 * @version (Curso 2018/19)
 */

class HashEntry<K, V> {
    
    protected K key;
    protected V value;

    HashEntry(K k, V v) {
        key = k;
        value = v;
    }
    
    public String toString() { 
        return "(" + key + ", " + value + ")";
    }
}