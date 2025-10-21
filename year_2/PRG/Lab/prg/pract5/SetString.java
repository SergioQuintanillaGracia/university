package pract5;
import java.util.Scanner;
/**
 * SetString class. Implementation of a set of String by means of 
 * a lexicographically ordered linked sequence.
 *
 * @author (PRG. ETSINF. UPV)
 * @version (Academic Year 2023/24)
 */
public class SetString {
    private NodeString first;
    private int size;
    
    /**
     * Constructor method that creates a new empty set.
     */
    public SetString() {
        first = null;
        size = 0;
    }

    /**
     * Method that adds s to the set.
     * If s belongs to the set, the set itself remains unchanged.
     * @param s String
     */
    public void add(String s) {
        NodeString curr = this.first;
        NodeString prev = null;
        
        while (curr != null && s.compareTo(curr.data) > 0) {
            prev = curr;
            curr = curr.next;
        }
        
        if (curr == null) {
            if (prev == null) {
                this.first = new NodeString(s);
            } else {
                prev.next = new NodeString(s);
            }
            
            size++;
        
        } else if (!curr.data.equals(s)) {
            if (prev == null) {
                first = new NodeString(s, curr);
            } else {
                prev.next = new NodeString(s, curr);
            }
            
            size++;
        }
    }
    
    /**
     * Checks if s belongs to the set.
     * @param s String.
     * @return returns true if and only if s belongs to the set..
     */
    public boolean contains(String s) {
        NodeString curr = first;
        
        while (curr != null) {
            if (curr.data.compareTo(s) > 0) {
                break;
            }
            
            if (curr.data.compareTo(s) == 0) {
                return true;
            }
            
            curr = curr.next;
        }
        
        return false;
    }

    /**
     * Removes s from the set
     * If s does not belong to the set, the set itself remains unchanged.
     * @param s String.
     */
    public void remove(String s) {
        NodeString curr = first;
        NodeString prev = null;
        
        while (curr != null) {
            if (curr.data.compareTo(s) > 0) {
                return;
            }
            
            if (curr.data.compareTo(s) == 0) {
                if (prev == null) {
                    first = curr.next;
                } else {
                    prev.next = curr.next;
                }
                
                size--;
                return;
            }
            
            prev = curr;
            curr = curr.next;
        }
    }
    
    /**
     * Returns the set size (or cardinal).
     * @return int the set size.
     */
    public int size() { 
        return size;
    }    
    
    /**
     * Returns the set resulting of the intersection of the current set 
     * and the argument set.
     * @param other SetString.
     * @return the resulting interseccion set.
     */
    public SetString intersection(SetString other) {
        SetString result = new SetString();
        NodeString n1 = this.first;
        NodeString n2 = other.first;
        NodeString last = null;  // Last node of the new set
        
        while (n1 != null && n2 != null) {
            if (n1.data.equals(n2.data)) {
                if (last == null) {
                    result.first = last = new NodeString(n1.data);
                } else {
                    last.next = new NodeString(n1.data);
                    last = last.next;
                }
                
                n1 = n1.next;
                n2 = n2.next;
                result.size++;
            } else {
                if (n1.data.compareTo(n2.data) < 0) {
                    n1 = n1.next;
                } else {
                    n2 = n2.next;
                }
            }
        }
        
        return result;
    }
    
    /** Solution 1
     * Returns the set resulting of the union of the current set and the 
     * argument set .
     * @param other SetString.
     * @return the resulting union set.
     */
    public SetString union(SetString other) {
        SetString result = new SetString();
        NodeString aux1 = this.first;
        NodeString aux2 = other.first;
        NodeString lastResult = null;
        while (aux1 != null && aux2 != null) {
            String s1 = aux1.data, s2 = aux2.data, s3 = null;
            int compare = s1.compareTo(s2);
            if (compare < 0) { s3 = s1; aux1 = aux1.next; }
            else if (compare > 0) { s3 = s2; aux2 = aux2.next; }
            else { s3 = s1; aux1 = aux1.next; aux2 = aux2.next; }
            
            // Insert s3 at the end of result:
            NodeString node = new NodeString(s3);
            if (lastResult == null) { result.first = node; }                   
            else { lastResult.next = node; }
            lastResult = node; 
            result.size++;
        }
        
        NodeString aux;
        if (aux1 != null) { aux = aux1; } else { aux = aux2; }
        while (aux != null) {
            String s = aux.data; aux = aux.next;
            NodeString node = new NodeString(s);
            if (lastResult == null) { result.first = node; }                   
            else { lastResult.next = node; }
            lastResult = node; 
            result.size++;
        }            
        return result;
    }
    
    /**
     * Method that returns a String with the set list of words 
     * in ascending order.
     * @return String with the list of words of the set
     */
    public String toString() {
        String result = "";
        NodeString aux = this.first;
        while (aux != null) {
            result += aux.data + "\n"; 
            aux = aux.next; 
        }
        return result;
    }
  
    /** 
     * Method that returns the SetString of the words read from 
     * Scanner s according to the separators by means of which
     * the Scanner has been configured.
     * @param s Scanner.
     * @return a new SetString of words read from Scanner s.
     */
    public static SetString setReading(Scanner s) {
        SetString setS = new SetString();
        while (s.hasNext()) {
            String word = s.next();
            setS.add(word);
        }
        return setS;
    }
    
}
