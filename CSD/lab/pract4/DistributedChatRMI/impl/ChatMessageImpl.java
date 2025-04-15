package impl;

import utils_rmi.ChatConfiguration;
import faces.IChatMessage;
import faces.IChatChannel;
import faces.IChatUser;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

/**
 * Simple IChatMessage implementation
 * 
 * <p>To understand this code, review interface IChatMessage first.
 * 
 * @author Pablo Galdamez
 */

public class ChatMessageImpl 
        extends UnicastRemoteObject 
        implements IChatMessage 
{
    private String str = ""; // The message
    private IChatUser src = null; // Message sender
    private Remote dst = null; // Message destination
    
    /**
     * Constructor to build a ChatMessage to be sent to a Channel
     * 
     * @param src Sender
     * @param dst Destination channel
     * @param str Text to send
     * @throws RemoteException Error
     */
    
    public ChatMessageImpl (IChatUser src, IChatChannel dst, String str) throws RemoteException
    {
        super (ChatConfiguration.the().getMyPort());
        this.src = src; this.dst = dst; this.str = str; 
    }
    
    /**
     * Constructor to build a ChatMessage to be sent to other user (private message)
     * 
     * @param src Sender
     * @param dst Destination user
     * @param str Text to send
     * @throws RemoteException Error
     */
    
    public ChatMessageImpl (IChatUser src, IChatUser dst, String str) throws RemoteException {
        super (ChatConfiguration.the().getMyPort());
        this.src = src; this.dst = dst; this.str = str; 
    }
    
    //
    // ISA IChatMessage
    //
    @Override
    public String getText () throws RemoteException {return str;}

    //
    // ISA IChatMessage
    //
    @Override
    public IChatUser getSender () throws RemoteException {return src;}

    //
    // ISA IChatMessage
    //
    @Override
    public Remote getDestination () throws RemoteException {return dst;}

    //
    // ISA IChatMessage
    //
    @Override
    public boolean isPrivate() {return dst instanceof IChatUser;}
}
