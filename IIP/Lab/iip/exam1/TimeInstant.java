package exam1;

/**
 * Class TimeInstant. 
 * This class allows you to represent instants or timestamps with 
 * detail of hours and minutes. Thus, this class represents the moment 
 * that defines a time instant, in this case, the hours and minutes of 
 * any given day.
 * 
 *  @author IIP 
 *  @version Academic Year 2023-24
 */
public class TimeInstant {

    // ATTRIBUTES:
    /** Integer attribute for storing the hours of a TimeInstant. 
     *  It should belong to the iterval <code>[0..23]</code>
     */
    private int hours;
    /** Integer attribute for storing the minutes of a TimeInstant. 
     *  It should belong to the iterval <code>[0..59]</code> 
     */
    private int minutes;
    
    // CONSTRUCTORS:
    /**
     *  Creates a TimeInstant corresponding to iniHours hours and 
     * iniMinutes minutes.
     * Precondition: 0 <= iniHours < 24, 0 <= iniMinutes < 60
     *
     * @param iniHours    Initial value for hours.
     * @param iniMinutes  Initial value for minutes.
     */
    public TimeInstant(int iniHours, int iniMinutes) {
        hours = iniHours;
        minutes = iniMinutes;
    }
    

    /** Returns number of minutes from 00:00 until current TimeInstant object
     *  @return The minutes from 00:00
     */
    public int toMinutes() {
        return hours * 60 + minutes;
    }
   
    /** Returns current TimeInstant object in "hh:mm" format.
     * @return A string representation of the current object.
     */
    public String toString() {
        String hh = "0" + hours;
        hh = hh.substring(hh.length() - 2);
        String mm = "0" + minutes;
        mm = mm.substring(mm.length() - 2);
        return hh + ":" + mm;
    }
   
    /** updates the hours and minutes of the current TimeInstant (this) 
     * dividing them by 2. To divide by 2, the TimeInstant is converted 
     * to minutes and these are divided by 2. 
     * From these minutes, the equivalent hours and minutes can be 
     * obtained to update the hours and minutes of this. 
     * For example, 13:10, dividing the hours and minutes by 2, 
     * would be 06:35 (13:10 equals 790 minutes; dividing by 2 is 
     * 395 minutes, which corresponds to 6 hours and 35 minutes).
     */
    public void divideBy2() {
        int newTotalMins = this.toMinutes() / 2;
        this.hours = newTotalMins / 60;
        this.minutes = newTotalMins % 60;
    }
}
