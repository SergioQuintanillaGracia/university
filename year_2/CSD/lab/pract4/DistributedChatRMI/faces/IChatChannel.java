package faces;

import java.rmi.Remote;
import java.rmi.RemoteException;

/**
 * Remote interface for a chat channel
 * 
 * <p>Channels have a set of connected users. Sending a message to a channel means to broadcast 
 * that message the channel users.
 * 
 * <p>Notice channels have no "security" at all. We can easily "kick" users.
 * 
 * @author Pablo Galdamez
 */

public interface IChatChannel extends Remote {
    
    /**
     * Join a channel. 
     * 
     * @param usr User that joins
     * @return List of users in the channel, including the new user.
     * 
     * @throws RemoteException failed to join
     */
    public IChatUser [] join (IChatUser usr) throws RemoteException;

    
    /**
     * Leave a channel
     * 
     * @param usr user that leaves.
     * 
     * @throws RemoteException Error levaing the channel
     */
    public void leave (IChatUser usr) throws RemoteException;

    
    /**
     * Send a message to the channel.
     * 
     * <p>All users in the channel will receive the message.
     * 
     * @param msg message to send 
     * @throws RemoteException Error sending the message
     */
    public void sendMessage (IChatMessage msg) throws RemoteException;

    
    /**
     * Get the channel name
     * 
     * @return the channel name
     * 
     * @throws RemoteException Error
     */
    public String getName () throws RemoteException;
}
