package pract7;

import pract5.DateTime;
/**
 * Calendar class: data type class representing the calendar of a venue where events are held.
 *
 * @author IIP - Lab Activity 7
 * @version Academic Year 2023/24
 */
public class Calendar {
    /* Attribute declaration*/
    public static final int MAX_EVENTS = 30;
    private int numEvents;
    private Event program[];
    private int numPerType[];
    
    /** Default constructor to create a new Calendar object empty (without events). */
    public Calendar() {      
        program = new Event[MAX_EVENTS];
        numEvents = 0;
        numPerType = new int[4];
    }
     
    /** 
     * Getter Method of the instance attribute numEvents. Return the total number of events.
     *  @return int, the number of events.
     */
    public int getNumEvents() { return numEvents; }
    
    /** 
     * Getter Method of the number of events of a given type, where 1 <= type <= 3.
     *  @param type int, the type. Precondition: 1 <= type <= 3.
     *  @return int, the number of events ot this type.
     */
    public int getNumType(int type) { return numPerType[type]; }
    
    /** 
     * Method toString, returns the String representation of all the events of the calendar.
     *  @return String.
     */
    public String toString() {
        String res = "";
        
        for (Event e : program) {
            res += e.toString() + '\n';
        }
        
        return res;
    }
    
    /** 
     * Method that returns true if a given Event e overlaps with any other Event 
     *  in the current calendar. Otherwise, it returns false.
     * @param e Event, the event.
     * @return boolean, true if e overlaps and false otherwise.
     */
    private boolean overlaps(Event inputE) {
        for (Event e : program) {
            if (UtilEvent.overlaps(e, inputE))
                return true;
        }
        
        return false;
    }
    
    /** 
     * Method that, if a given Event e fits and does not overlap with any other
     *   in the current calendar, adds it in chronological order to the calendar and returns true. 
     *   If it does not fit or there is overlap, it does not add it and returns false.
     * @param e Event, event to be added.  
     * @return boolean, true if added and false otherwise.
     */    
    public boolean add(Event e) { 
        if (numEvents >= MAX_EVENTS || overlaps(e)) {
            return false;
        }
        
        int pos = 0;
        DateTime eDate = e.getStartTime();
        
        for (int i = numEvents - 1; i >= 0; i--) {
            DateTime currDate = program[i].getStartTime();
            pos = i;
            
            if (eDate.compareTo(currDate) < 0) {
                // The given event is earlier than program[i];
                pos = i + 1;
                break;
            }
        }
        
        // Move the elements at pos and at the right one to the right.
        for (int i = numEvents; i >= pos; i++) {
            program[i + 1] = program[i];
        }
        
        // Place the input event at its position.
        program[pos] = e;
        
        numEvents++;
        numPerType[e.getType()]++;
        
        return true; 
    }
    
    /** 
     * Method that returns the position in the array program for a given title event.
     * or -1 if it does not exist.
     * @param title String, the title.
     * @return int, the position of the event in the array or -1 if not present. 
     */
    private int searchTitle(String title) {
        
        return -1;  
    }
           
    /** 
     * Method that removes the given title event from the calendar, if it exists, and returns true.
     * If it does not exist, removes nothing and returns false.
     * @param title String, the title.
     * @return boolean, true if removed and false otherwise.     * @param title String, the title.
     * @return int, the event's position in the array, or -1 if it is not present. 
     */
    public boolean delete(String title) {
        /* TO COMPLETE */
        return true;
    }
    
    /** 
     * Method that returns the longest event (with the longest duration) in the calendar.
     * Precondition: at least one event. 
     * @return Event, the event.
     */
    public Event eventLarger() {
        /* TO COMPLETE */
        return null;
    }
    
    /** 
     * Method that returns an array with all the events of type type type of the calendar, being 1 <= type <= 3. 
     * The size of the returned array must be equal to the number of events of type type, 
     * being 0 if there are no events of type type type.
     * @param type int, the type. Precondition: 1 <= type <= 3.
     * @return Event[], the array.
     */
    public Event[] filterByType(int type) {
        /* TO COMPLETE */
        return null;
    }
    
}
