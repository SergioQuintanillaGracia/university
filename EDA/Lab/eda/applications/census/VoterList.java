package applications.census;

import libraries.dataStructures.models.ListPOI;
import libraries.dataStructures.linear.LinkedListPOI;
import libraries.dataStructures.linear.SortedLinkedListPOI;

/**
 * VoterList: represents a list of residents registered in
 *            the census, and, thus, voters
 *
 * @author  EDA Professors
 * @version September 2023 (Translation Feb. 24)
 */

public class VoterList {

    private ListPOI<Resident> census;
    private int size;

    /**
     * Field getter methods
     */
    public ListPOI<Resident> getCensus() { return census; }
    public int getSize() { return size; }

    /**
     * Returns the String that represents a VoterList
     *
     * @return the String with the VoterList in the given textual format.
     */
    public String toString() {
        String res = "";
        if (size == 0) return res;
        census.begin();
        for (int pos = 0; pos <= census.size() - 2; pos++) {
            res += census.get() + ", \n";
            census.next();
        }
        res += census.get();
        return res;
    }

    /**
     * Creates a VoterList...
     *
     * @param sorted A boolean that shows whether the census must be
     *               sorted in ascending order (true) or not (false).
     * @param n     An int that shows the size (number of elements) of the list
     */
    public VoterList(boolean sorted, int n) {
        size = n;
        
        if (sorted) {
            census = new SortedLinkedListPOI();
        } else {
            census = new LinkedListPOI();
        }
        
        int inserted = 0;
        while (inserted < n) {
            Resident r = new Resident();
            if (index(r) == -1) {
                census.add(r);
                inserted++;
            }
        }
    }

    /**
     * Returns the index or position of Resident r in a VoterList,
     * or -1 if r doesn't belong to the list.
     *
     * @param r a Resident
     * @return r's index in a census, an int, 0 or positive
     *         if r is in the census, or -1 otherwise.
     */
    protected int index(Resident r) {
        census.begin();
        int i = 0;
        
        while (!census.isEnd()) {
            if (census.get().equals(r)) return i;
            census.next();
            i++;
        }
        
        return -1;
    }
    
    public boolean remove(Resident r) {
        census.begin();
        
        // Try to find the resident in the census
        while (!census.isEnd()) {
            if (census.get().equals(r)) {
                // If the current resident in the census is equal to the
                // passed resident, remove it and reduce the size accordingly
                census.remove();
                size--;
                return true;
            }
            
            census.next();
        }
        
        // We got to the end of the census and the resident wasn't found in
        // it, return false
        return false;
    }
    
    public static void testRemove() {
        VoterList vl = new VoterList(true, 4);
        System.out.println(vl);
        vl.census.begin();
        vl.census.next();
        vl.remove(vl.census.get());
        System.out.println("\nRemoving the second resident...");
        System.out.println(vl);
        vl.census.begin();
        vl.remove(vl.census.get());
        System.out.println("\nRemoving the first resident...");
        System.out.println(vl);
        vl.census.begin();
        vl.census.next();
        vl.remove(vl.census.get());
        System.out.println("\nRemoving the last resident...");
        System.out.println(vl);
    }
}
