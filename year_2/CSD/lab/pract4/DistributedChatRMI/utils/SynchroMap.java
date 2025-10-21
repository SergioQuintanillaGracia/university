package utils;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;


/**
 * Thread-safe hash map.
 * 
 * <p> Trivial synchronized implementaion of a map. Delegates calls to HashMap.
 * 
 * <p>It's same as Java's synchronzied version of HashMap, but includes a couple
 * of methods to get a copy of the contents.
 * 
 * <p>Notice that Java's synchronized version of Map cannot be extended.
 * 
 * @author Pablo Galdamez
 * 
 * @param <K> Type for keys
 * @param <T> Type for Values
 */

public class SynchroMap <K, T> 
        implements Map<K,T>
{
    private final Map<K,T> map = new HashMap<> ();
    
    public synchronized Map<K,T> copyOf () {
        Map<K,T> copy = new HashMap<>(size());
        copy.putAll (map);
        return copy;
    }
     
    public synchronized List<T> copyOfValues () {
        List<T> copy = new ArrayList<> (size());
        copy.addAll (map.values());
        return copy;
    }
    
    @Override
    public synchronized int size() {
        return map.size();
    }

    @Override
    public synchronized boolean isEmpty() {
        return map.isEmpty();
    }

    @Override
    public synchronized boolean containsKey(Object o) {
        return map.containsKey(o);
    }

    @Override
    public synchronized boolean containsValue(Object o) {
        return map.containsValue(o);
    }

    @Override
    public synchronized T get(Object o) {
        return map.get (o);
    }

    @Override
    public synchronized T put(K key, T value) {
        return map.put(key, value);
    }

    @Override
    public synchronized T remove(Object o) {
        return map.remove(o);
    }

    @Override
    public synchronized void putAll(Map<? extends K, ? extends T> m) {
        map.putAll(m);
    }

    @Override
    public synchronized void clear() {
        map.clear();
    }

    @Override
    public synchronized Set<K> keySet() {
        return map.keySet();
    }

    @Override
    public synchronized Collection<T> values() {
        return map.values();
    }

    @Override
    public synchronized Set<Entry<K, T>> entrySet() {
        return map.entrySet ();
    }
         
}
