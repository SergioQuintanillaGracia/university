package ui;

/**
 * Interface for a ChatClient.
 * 
 * <p>The graphical user interface can use these methods, when user does actions by 
 * clicking and typing into the user interface.
 * 
 * <p>As of now we have 2 different programs that implement this interface: the
 * ChatClientRMI (practice 4) and the ChatClientJMS (practice 5)
 * 
 * <p>Notice this is not a Remote object. It is a simple Java interface.
 * 
 * @author Pablo Galdamez
 */

public interface ChatClientFace {

    /**
     * Connect to some specific chat server.
     * 
     * <p>The first thing to do before chatting is to connect to a ChatServer!!
     * 
     * <p>On success, returns the server channel list.
     * 
     * @param serverName server name we connect to
     * @param nick nick name for our user
     * 
     * @return channel list as list of channel names
     * 
     * @throws Exception error on connection
     */    
    public String [] doConnect (String serverName, String nick) throws Exception;

    
    /**
     * Disconnect 
     * 
     * <p> User interface commands to disconnect to terminate chatting session.
     * 
     * <p> Disconnect allows server to free up resources
     */    
    public void doDisconnect ();
    
    
    /**
     * Leave a chat channel
     * 
     * <p>When users leave a chat lounge they exit the Channel using this method.
     * 
     * @param channelName channel to abandon
     * @throws Exception Error
     */
    public void doLeaveChannel (String channelName) throws Exception;
    
    
    /**
     * Join a channel
     * 
     * <p>To chat in a channel we require users to join.
     * 
     * <p>On success, returns the user list
     *
     * @param channelName channel to join
     * 
     * @return the users list connected to this channel, as string list of nick names
     * @throws Exception Error joining
     */
    public String [] doJoinChannel (String channelName) throws Exception;
    
    
    /**
     * Send a message to a channel
     * 
     * @param dst channel name
     * @param msg text message to sent
     * @throws Exception Error sending the message
     */
    public void doSendChannelMessage (String dst, String msg) throws Exception;
    
    /**
     * Send a message to a specific user
     * 
     * @param dst nick name for the destination user
     * @param msg text message to send
     * @throws Exception Error sending the message
     */
    public void doSendPrivateMessage (String dst, String msg) throws Exception;
    
    /**
     * UI commands to terminate the Chat Program.
     */
    public void doTerminate ();    
}
