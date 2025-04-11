package applications.hospital;


/**
 * Write a description of class PriorityQueueServerPlus here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class PriorityQueueServerPlus extends PriorityQueueServer
                                     implements SurgeryServerPlus {
    private int size;
                                         
    /**
     * Constructor for objects of class PriorityQueueServerPlus
     */
    public PriorityQueueServerPlus() {
        super();
        size = 0;
    }
    
    public int numPatients() {
        return size;
    }
    
    public Patient transferPatient() {
        Patient p = pQ.removeMin();
        size--;
        return p;
    }
    
    @Override
    public void addWaiting(Patient p) {
        super.addWaiting(p);
        size++;
    }
    
    @Override
    public Patient operatePatient(int h) {
        size--;
        return super.operatePatient(h);
    }
    
    public void distributePatients(SurgeryServerPlus s) {
        Patient[] arr = new Patient[size];
        
        // Move patients to the array
        int index = 0;
        while (hasPatients()) {
            arr[index] = transferPatient();
        }
        
        // Add patients to each surgery server alternatively
        for (int i = 0; i < arr.length; i++) {
            if (i % 2 == 0) {
                addWaiting(arr[i]);
            } else {
                s.addWaiting(arr[i]);
            }
        }
    }
    
    
}
