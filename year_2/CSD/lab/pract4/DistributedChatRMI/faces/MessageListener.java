package faces;

/**
 * Simple interface to be implemented by Chat Client programs.
 * 
 * <p>Notice this is not a Remote object. It is a simple Java interface.
 * 
 * @author Pablo Galdamez
 */

public interface MessageListener {
    
    /**
     * Callback to run when a message arrives.
     * 
     * @param msg message that has arrived.
     */
    
    public void messageArrived (IChatMessage msg);
}
