package applications.hospital;

import libraries.dataStructures.linear.ArrayQueue;
import libraries.dataStructures.models.Queue;

/**
 * FIFOQueueServer class: implements a SurgeryServer that
 * uses a FIFO model (Queue) to handle waiting patients.
 *
 *  @author  EDA Professors
 *  @version April 2024
 */

public class FIFOQueueServer implements SurgeryServer {

    /** A FIFOQueueServer HAS A Queue q of Patients to be operated. */
    protected Queue<Patient> q;

    /** Creates an empty server. */
    public FIFOQueueServer() { q = new ArrayQueue<>(); }

    /** Includes a new Patient p in a SurgeryServer. */
    @Override
    public void addWaiting(Patient p) { q.enqueue(p); }

    /** Checks whether there is any Patient in this SurgeryServer. */
    @Override
    public boolean hasPatients() { return !q.isEmpty(); }

    /** IFF hasPatients(): returns the next Patient to be operated on. */
    @Override
    public Patient getPatient() { return q.first(); }

    /**
     * IFF hasPatients(): removes from a SurgeryServer the Patient that
     * will be operated on, and returns that Patient, updating its
     * entersSurgery attribute.
     * @param h Timestamp (in hours) when the patient is admitted to surgery.
     */
    @Override
    public Patient operatePatient(int h) {
        Patient p = q.dequeue();
        p.setEntersSurgery(h + SURGERY_TIME);
        return p;
    }
}
