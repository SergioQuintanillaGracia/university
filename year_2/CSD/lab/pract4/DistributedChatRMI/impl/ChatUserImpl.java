package impl;

import utils_rmi.ChatConfiguration;
import faces.IChatMessage;
import faces.IChatUser;
import faces.MessageListener;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

/**
 * Simple IChatUser implementation. 
 * 
 * <p>To understand this code, review interface IChatMessage first.
 * 
 * <p>Notice how this implementation just calls a listener on each message.
 * 
 * @author Pablo Galdamez
 */
public class ChatUserImpl 
        extends UnicastRemoteObject 
        implements IChatUser 
{
    private final String nick;
    private final MessageListener lis;
    
    /**
     * Constructor for ChatUserImpl. It needs a nick and a listener where
     * messages will go.
     * 
     * @param nick user name
     * @param lis listener for messages
     * 
     * @throws RemoteException Error
     */
    public ChatUserImpl (String nick, MessageListener lis) throws RemoteException {
        super (ChatConfiguration.the().getMyPort());
        this.nick = nick.trim();
        this.lis = lis;
    }

    //
    // ISA IChatUser
    //
    @Override
    public String getNick() throws RemoteException {
        return nick;
    } 
    
    //
    // ISA IChatUser
    //
    // To send a message we simply send the message to the listener.
    //
    @Override
    public void sendMessage (IChatMessage msg) throws RemoteException {
        lis.messageArrived (msg);
    }
}