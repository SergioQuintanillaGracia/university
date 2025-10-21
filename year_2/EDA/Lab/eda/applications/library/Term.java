package applications.library;

/**
 * Term: Key class of a Map that represents a word or term of
 * the Analytical Index of a Digital Library.
 *
 * To override efficiently the equals and hashCode methods of Object,
 * each Term HAS A hashValue that stores the result of the FIRST
 * invocation of the hashCode method.
 * Thus, the hash value associated to a given Term...
 ** (a) is only computed once, regardless of the number of
 **     calls to hashCode;
 ** (b) can be used in the equals method of the class to ONLY check
 **     for equality if the hash values are equal.
 *
 * On top of that, to evaluate different hashCode implementations, each
 * Term HAS A hashCodeBase that stores the numerical base to use when
 * computing its hashValue.
 * 
 * @author (EDA-QA) 
 * @version (Curso 2020-2021)
 */

public class Term {
    
    public static final int TRIVIAL_BASE = 1;
    public static final int JAVA_LANG_STRING_BASE = 31;
    public static final int MCKENZIE_BASE = 4;
    
    protected String word;
    // for efficiency reasons: "caching the hash code" o "hash cache"
    protected int hashValue;
    // to evaluate different hashCode() implementations, with different bases
    protected int hashCodeBase;

    /** Creates the Term associated to a word t and selects
     *  the base to be used in the hashCode method. */
    public Term(String t, int base) {
        word = new String(t);
        hashCodeBase = base;
        hashValue = 0;
    }
    
    /** Creates the Term associated to a word t according to Java's standard. */
    public Term(String t) {
        this(t, JAVA_LANG_STRING_BASE);
    }

    /** Return the hash value of this Term in an EFFICIENT way, i.e.
     *  when running this method for the FIRST time over a Term of length
     *  n (this.word.length()), computes its hash value using the following
     *  polynomial equation. It must be implemented using Horner's method,
     *  so WITHOUT using methods from the Math class.
     *  this.hashValue = word.charAt(0) * hashCodeBase ^ (n - 1)
     *                 + word.charAt(1) * hashCodeBase ^ (n - 2)
     *                 + ...
     *                 + word.chatAt(n - 1)
     *  If this ISN'T the first call to this method, returns this.hashValue
     */
    public int hashCode() { 
        int res = this.hashValue;
        if (res != 0) { return res; }

        // compute the value of res for this.word in a given this.hashCodeBase
        // in an EFFICIENT way, i.e., using Horner's method.
        for (int i = 0; i < word.length(); i++) {
            res = res * hashCodeBase + word.charAt(i);
        }
        
        this.hashValue = res;
        return res;
    }

    /** Checks whether this Term is equal to another in an efficient
     *  way, i.e., ONLY runs the equals method of String when the
     *  hash values of this and other are equal.
     */
    public boolean equals(Object other) {
        if (!(other instanceof Term)) return false;
        Term o = (Term) other;
        
        return hashCode() == o.hashCode() && word.equals(o.word);
    }
    
    /** Returns a String that represents a Term in a given textual format */
    public String toString() { return word + " (" + hashValue + ")\n"; }
}
