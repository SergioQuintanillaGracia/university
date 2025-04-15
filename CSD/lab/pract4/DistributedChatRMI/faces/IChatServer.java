package faces;

import java.rmi.Remote;
import java.rmi.RemoteException;

/**
 * Remote interface for a chat server.
 * 
 * <p>ChatServers have a set of channels and a set of connected users.
 * 
 * <p>Notice there is no "security" at all. It's easy to "kick" or "impersonate" users.
 * 
 * @author Pablo Galdamez
 */
public interface IChatServer extends Remote {
    
    /**
     * List server channels. Exception if problems in the server.
     * 
     * @return the channels list
     * 
     * @throws RemoteException Error
     */    
    public IChatChannel [] listChannels () throws RemoteException;

    
    /**
     * Get a channel from the server. Null if not found, exception on problems.
     * 
     * @param name channel name
     * @return Channel if found, null if not found.
     * 
     * @throws RemoteException Error
     */
    public IChatChannel getChannel (String name) throws RemoteException;

    
    /**
     * Create a new channel inside the server. 
     * 
     * <p>Exception if already exist or any other problem.
     * 
     * @param name channel name
     * 
     * @return newly created channel
     * @throws RemoteException Error creating the channel
     */    
    public IChatChannel createChannel (String name) throws RemoteException;


    /**
     * Get user for a given nick. 
     * 
     * @param nick user name
     * @return user if found, null if not found
     * 
     * @throws RemoteException Error
     */    
    public IChatUser getUser (String nick) throws RemoteException;

    
    /**
     * Connects to ChatServer. Exception on any problem connecting.
     * 
     * @param usr user to connect
     * @throws RemoteException Error connecting
     */    
    public void connectUser (IChatUser usr) throws RemoteException;
    
    
    /**
     * Disconnect user. Exception on problems.
     * 
     * @param usr user to disconnect
     * @throws RemoteException Error disconnecting
     */
    public void disconnectUser (IChatUser usr) throws RemoteException;
}
