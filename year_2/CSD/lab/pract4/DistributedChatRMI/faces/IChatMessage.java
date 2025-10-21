package faces;

import java.rmi.Remote;
import java.rmi.RemoteException;

/**
 * Remote interface for a chat message
 * 
 * <p>In our chat system, messages are remote objects. We could simplify the chat system so that 
 * messages would be simple strings. Hoewever, having them as objects allows us to explore interesting 
 * invocation patterns.
 * 
 * @author Pablo Galdamez
 */

public interface IChatMessage extends Remote{

    /**
     * Get the message contents as a String.
     * 
     * @return the message string
     * 
     * @throws RemoteException Error
     */
    public String getText () throws RemoteException;    

    
    /**
     * Get the user who sent the message
     * 
     * @return the sender
     * 
     * @throws RemoteException Error
     */
    public IChatUser getSender () throws RemoteException; 

    
    /**
     * Get the message destination. Destination can be a IChatChannel or IChatUSer
     * Since it could be any of them, this method returns a generic "Remote".
     * 
     * When using this method we must check with "isPrivate()" method whether
     * destination is IChatUser (private) or IChatChannel (not private)
     * 
     * @return message destination
     * 
     * @throws RemoteException Error
     */
    public Remote getDestination () throws RemoteException; 

    
    /**
     * Return whether destination is private (user) or not private (channel)
     * 
     * @return true for private message, false otherwise
     * 
     * @throws RemoteException Error
     */    
    public boolean isPrivate() throws RemoteException; 
}
