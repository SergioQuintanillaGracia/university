package applications.hospital;

/**
 * SurgeryServerPlus interface: additional functionality.
 *
 * @author  EDA Professors
 * @version April 2024
 */
public interface SurgeryServerPlus extends SurgeryServer {
    /** Obtains the number of patients currently waiting on this server. */
    int numPatients();

    /** Removes the next patient to be operated, so that it can be
     *  transferred to another server. */
    Patient transferPatient();

    /** Splits the patients between this and the given server s. */
    void distributePatients(SurgeryServerPlus s);
}
