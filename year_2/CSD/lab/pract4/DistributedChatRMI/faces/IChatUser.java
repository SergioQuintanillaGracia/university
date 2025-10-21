package faces;

import java.rmi.Remote;
import java.rmi.RemoteException;

/**
 * Remote interface for a chat user.
 * 
 * <p>Users are objects. They receive messages when they are invoked their "sendMessage()" 
 * 
 * @author Pablo Galdamez
 */
public interface IChatUser extends Remote {
    
    /**
     * Get the user nick
     * 
     * @return the nick
     * 
     * @throws RemoteException Error
     */
    public String getNick() throws RemoteException;
    
    /**
     * Send a message to this user
     * 
     * @param msg message to send.
     * 
     * @throws RemoteException Error sending the message
     */
    public void sendMessage (IChatMessage msg) throws RemoteException;
}
