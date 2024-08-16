package pract2;

/**
 * Clas PRGString: class of tools with methods for working with Strings.
 * 
 * @author PRG - ETSINF - DSIC - UPV
 * @version Academic Year 2023/2024
 */
public class PRGString {
    
    /** Objects of this class cannot be created. */
    private PRGString() { }
    
    /**
     * Returns the number of occurrences of the letter 'a' in the String given as parameter.
     * @param s String to be explored to find occurrences of the letter 'a'.
     * @return int
     */
    public static int countA(String s) {
        // Trivial case: Empty String
        if (s.length() == 0) { return 0; }
        // General case: Non empty String. Check with the next substring.
        else if (s.charAt(0) == 'a') { return 1 + countA(s.substring(1)); }
        else { return countA(s.substring(1)); }
    }

    /**
     * Returns the number of occurrences of the letter 'a' in
     * the String given as parameter from the specified position.
     * @param s String to be explored to find occurrences of the letter 'a'.
     * @param pos starting position of 's' where the substring starts.
     * @return int
     * PRECONDITION: pos >= 0
     */
    public static int countA(String s, int pos) {
        // Trivial case: Empty string
        if (pos >= s.length()) { return 0; }
        // General case: Non empty String. Check with the next substring.
        else if (s.charAt(pos) == 'a') { return 1 + countA(s, pos + 1); }
        else { return countA(s, pos + 1); }
    }

    /**
     * Returns the number of occurrences of the letter 'a' in
     * @param s String to be explored to find occurrences of the letter 'a'.
     * @return int
     */
    public static int countA2(String s) {
        // Trivial case: Empty String
        if (s.length() == 0) { return 0; }
        // General case: Non emtpy String. Check with the next substring.
        else if (s.charAt(s.length() - 1) == 'a') {
            return 1 + countA2(s.substring(0, s.length() - 1));
        } else { return countA2(s.substring(0, s.length() - 1)); }
    }

    /**
     * Check whether 'a' is prefix of 'b'.
     * -- TO BE COMPLETED --
     */
    public static boolean isPrefix(String a, String b) {
        if (a.isEmpty()) {
            return true;
        } else {
            if (b.isEmpty()) {
                return false;
            } else {
                if (a.charAt(0) == b.charAt(0)) {
                    return isPrefix(a.substring(1), b.substring(1));
                } else {
                    return false;
                }
            }
        }
    }

    /**
     * Check whether 'a' is a substring of 'b'.
     * -- TO BE COMPLETED --
     */
    public static boolean isSubstring(String a, String b) {
        // Long form:
        // if (a.isEmpty()) return true;
        // if (b.isEmpty()) return false;
        // if (isPrefix(a, b)) return true;
        // return isSubstring(a, b.substring(1));
        
        // Short form:
        if (a.isEmpty() || isPrefix(a, b)) return true;
        if (b.isEmpty()) return false;
        return isSubstring(a, b.substring(1));
    }
}