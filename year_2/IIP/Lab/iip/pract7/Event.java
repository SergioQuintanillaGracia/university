package pract7;

import pract5.DateTime;
/**
 * Class Event: data type class representing an event with a title, 
 * of a given type and with a start date and an end date.
 *
 * @author IIP - Lab Activity 7
 * @version Academic year 2023/24
 */
public class Event {
    public static final int CONGRESS = 1, SHOW = 2, EXHIBITION = 3;    
    private DateTime startTime, endTime;
    private String title;
    private int type;
    
    /**
     * General constructor of class Event.
     *  @param start DateTime, start date (previous to end date).
     *  @param end DateTime, end date (after start date).
     *  @param tit String, the title.
     *  @param type int, the type of el tipo, 1 <= type <= 3.
     */
    public Event(DateTime start, DateTime end, String tit, int type) {
        startTime = start;
        endTime = end;
        title = tit;
        this.type = type;
    }
    
    /**
     * Getter Method of the start date.
     *  @return DateTime, start date.
     */
    public DateTime getStartTime() { return startTime; }
    
    /**
     * Getter Method of the end date.
     *  @return DateTime, end date.
     */
    public DateTime getEndTime() { return endTime; }
    
    /**
     * Getter Method of the type.
     *  @return int, el tipo.
     */
    public int getType() { return type; }
        
    /**
     * Getter Method of the title.
     *  @return String, the title.
     */
    public String getTitle() { return title; }  
    
    /**
     * Setter Method of the start date.
     *  @param d DateTime, new start date.
     */
    public void setStartTime(DateTime d) { startTime = d; }
    
    /**
     * Setter Method of the end date.
     *  @param d DateTime, new end date.
     */
    public void setEndTime(DateTime d) { endTime = d; }
    
    /**
     * Setter Method of the type .
     *  @param t int, new Type, where 1 <= t <= 3.
     */
    public void setType(int t) { type = t; }
    
    /**
     * Setter Method of the title.
     *  @param t String, new title.
     */
    public void setTitle(String t) { title = t; }
    
    /**
     * Getter of the duration in minutes.
     *  @return int, minutes from the start date to the end date.
     */
    public int getDuration() {
        return UtilEvent.getDuration(startTime, endTime);
    }
    
    /**
     * Method toString, returns a String as a description of the current Event, according to
     * the following format: first line contains the title, next line contains the initial 
     * date, a hyphen and the final date, and finally, the last line contains inside 
     * parenthesis a word in plural and upper case with the type of the event, (CONGRESSES), 
     * (SHOWS), (EXHIBITIONS)} finishing with a "\n"
     *  @return String, the description
     */
    public String toString() {
        String res = title + "\n" + startTime + " - " + endTime + "\n";
        switch (type) {
            case CONGRESS: 
                res += "(CONGRESSES)\n"; break;
            case SHOW: 
                res += "(SHOWS)\n"; break; 
            default:
                res += "(EXHIBITIONS)\n";
        }
        return res;
    }
    
    /**
     * Method equals, check the equality of the current event with the given object.
     * @param o Object.
     * @return boolean, true if the given object if of class Event, and title, type 
     *      and start and end dates are the same of the given object, false otherwise 
     */
    public boolean equals(Object o) {
        return o instanceof Event
            && title.equals(((Event) o).title)
            && type == ((Event) o).type
            && startTime.equals(((Event) o).startTime)
            && endTime.equals(((Event) o).endTime);
    }
}
