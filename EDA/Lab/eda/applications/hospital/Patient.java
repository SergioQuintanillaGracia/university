package applications.hospital;

import applications.census.Resident;

/** Patient class: represents a patients to be operated.
 *
 * @author  EDA Professors
 * @version April 2024
 */
public class Patient implements Comparable<Patient> {

    private final Resident id;

    /** Type of surgery */
    private final String surgery;

    /**
     * Indicator of the gravity of the patient, i.e., the surgery's urgency.
     * Must be in the range [0..9], with higher value --> least urgent.
     */
    private final int gravity;

    /** Timestamp (in hours) for the entrance of the patient into the waiting list. */
    private final int entersWaitlist;

    /** Timestamp (in hours) for the start of the operation. */
    private int entersSurgery;

    /**
     * Creates a new Patient to be operated.
     * @param s Type of surgery.
     * @param g Gravity, in range [0..9].
     * @param t Time (in hours) when the patient arrives to the list.
     */
    public Patient(String s, int g, int t) {
        id = new Resident();
        surgery = s;
        gravity = g;
        entersWaitlist = t;
    }

    /** Returns the surgery associated to a Patient. */
    public String getSurgery() {
        return surgery;
    }

    /** Returns a Patient's gravity. */
    public int getGravity() {
        return gravity;
    }

    /** Returns the hour in which a Patient entered the waiting list. */
    public int getEntersWaitlist() {
        return entersWaitlist;
    }

    /** Returns the hour in which a Patient is operated. */
    public int getEntersSurgery() {
        return entersSurgery;
    }

    /** Sets the timestamp when the Patient entered surgery. */
    public void setEntersSurgery(int t) {
        entersSurgery = t;
    }

    /** Returns the amount of hours that a Patient waits for. */
    public int getDelay() {
        return entersSurgery - entersWaitlist;
    }

    /**
     * Returns an integer value that results from comparing the gravity
     * of a Patient (this) with other's. This value should be...
     * ** NEGATIVE if this Patient has MORE gravity than the other
     *    (MORE gravity -> lower value of the gravity attribute),
     *    i.e., its surgery is MORE urgent than the other's.
     * ** POSITIVE if this Patient has LESS gravity than the other,
     *    i.e., its surgery is LESS urgent than the other's.
     * ** ZERO if both patients have the same gravity.
     */
    @Override
    public int compareTo(Patient other) {
        return gravity - other.gravity;
    }

    /** Returns a string representation for this Patient. */
    @Override
    public String toString() {
        return String.format("%s (gravity: %d) \tWaited: %.1f DAYS.\t%s",
                surgery, gravity, getDelay() / 24.0, id);
    }
}
