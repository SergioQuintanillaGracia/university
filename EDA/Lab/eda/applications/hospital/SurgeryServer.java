package applications.hospital;

/**
 * SurgeryServer interface:
 * specifies the service capacity of a surgery server
 * and the methods that it must implement **independently**
 * of whether it uses a Queue or a PriorityQueue
 * to handle waiting patients.
 *
 * @author  EDA Professors
 * @version April 2024
 */
public interface SurgeryServer {
    public static final int SURGERY_TIME = 3; // 3 hours for any surgery

    /**
     * Includes a new Patient in the waiting list, in a SurgeryServer.
     */
    void addWaiting(Patient p);

    /**
     * Checks whether there is any patient waiting in a SurgeryServer.
     */
    boolean hasPatients();

    /**
     * IFF hasPatients(): returns the Patient from a SurgeryServer to be operated.
     */
    Patient getPatient();

    /**
     * IFF hasPatients(): removes from a SurgeryServer the Patient that
     * will be operated on, and returns that Patient, updating its
     * entersSurgery attribute.
     * @param h Timestamp (in hours) when the patient is admitted to surgery.
     */
    Patient operatePatient(int h);
}
