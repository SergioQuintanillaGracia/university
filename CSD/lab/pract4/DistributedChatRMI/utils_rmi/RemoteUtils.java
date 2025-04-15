package utils_rmi;

import java.rmi.Remote;

/**
 * This class is usefull aid to print the contents of Remote References.
 * 
 * @author Pablo Galdamez
 */

public class RemoteUtils {
    
    private RemoteUtils() {} 
    
    /**
     * Utility function. Return a human-readable string representation of a reference.
     * 
     * <p>Code is tricky and it is based on usual internal String representation of references.
     * 
     * <p>This functions gets a string representation finding relevant substrings inside the
     * Java's returned string. If any Java version changes the string representation, this
     * function won't crash, but it will simply return the unmodified Java's string version.
     * 
     * @param ref Remote reference.
     * @return String representation of a reference
     */

    public static String remote2String (Remote ref) {
        String str = ref == null ? "null" : ref.toString();
        int bidx1 = str.indexOf("[");
        int eidx1 = str.indexOf(",");
        int bidx2 = str.indexOf("endpoint");
        int eidx2 = str.indexOf("(remote)");
        try {
            String str1 = str.substring(bidx1+1, eidx1+1);
            String str2 = str.substring(bidx2, eidx2);
            str = "{" + str1 + str2 + "}";            
        } catch (Exception e) {}
        return str;
    }

}
