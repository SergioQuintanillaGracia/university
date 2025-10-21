package pract5;

/**
 * Class NodeString.
 *
 * @author (PRG. ETSINF. UPV)
 * @version (Academic Year 2023/24)
 */
public class NodeString {
    
    String  data;
    NodeString next; 
 
    /** 
     * Constructor method with a parameter that is assigned to the data 
     * attribute, initialising the next attribute to null
     * @param d String. value of the String of the new node.
     */
    NodeString(String d) {  
        this.data = d;
        this.next = null;
    } 
    
    /** 
     * Constructor method with two parameters, one for each attribute.
     * @param d String. Value of the String of the new node.
     * @param s NodeString. Value of the succesor node of the new node.
     */
    NodeString(String d, NodeString s) {
        this.data = d;
        this.next = s;
    }        
}
