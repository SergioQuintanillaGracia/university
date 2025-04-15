package impl;

import utils_rmi.ChatConfiguration;
import utils.SynchroMap;
import faces.IChatMessage;
import faces.IChatChannel;
import faces.IChatUser;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.Map;

/**
 * Simple IChatChannel implementation
 * 
 * <p>To understand this code, review interface IChatChannel first.
 * 
 * @author Pablo Galdamez
 */

public class ChatChannelImpl 
        extends UnicastRemoteObject 
        implements IChatChannel 
{
    public static final String LEAVE = "LEAVE";
    public static final String JOIN = "JOIN";
    
    private final String name;
    private final SynchroMap <String, IChatUser> users = new SynchroMap<> (); // Shared!!
    
    /**
     * Create a new ChatChannelImpl as RMI object.
     * It will be bound to the port specified by configuration.
     * 
     * @param name name for the channel
     * 
     * @throws RemoteException Error
     */
    public ChatChannelImpl (String name) throws RemoteException {
        super (ChatConfiguration.the().getMyPort());
        this.name = name.trim();        
    }
    
    //
    // ISA IChatChannel
    //
    @Override
    public String getName () throws RemoteException {
        return name;
    }
    
    //
    // ISA IChatChannel
    //
    @Override
    public IChatUser [] join (IChatUser usr) throws RemoteException
    {        
        purge ();        
        String nick = usr.getNick(); // Exception here sends me out.

        List<IChatUser> currentUsers;
        synchronized (users) { // Atomic "get+put"
            if (users.get (nick) != null) throw new RemoteException ("Nick '" + nick + "' already connected");
            users.put (nick, usr); 
            
            // All users, including the new will receive notification
            currentUsers = users.copyOfValues(); 
        }

        notifyUsers (currentUsers, JOIN, nick);
        return currentUsers.toArray( new IChatUser [currentUsers.size()]);
    }
    
    //
    // ISA IChatChannel
    //
    @Override
    public void leave (IChatUser usr) throws RemoteException 
    {
        purge ();       
        if (usr == null) return; // leave (null) means I'm just asked to purge        
        
        String nick = usr.getNick(); // If exception here, I'm out.
        
        List<IChatUser> currentUsers;        
        synchronized (users) {
            if (users.get (nick) == null) throw new RemoteException ("User '" + nick + "'not found");
            users.remove (nick);   
            currentUsers = users.copyOfValues();
        }
        
        // Channel sends a control message to all users at the channel---> one user left
        notifyUsers (currentUsers, LEAVE, nick);
    }
    
    //
    // ISA IChatChannel
    //
    @Override
    public void sendMessage (IChatMessage msg) throws RemoteException
    {
        purge ();   
        if (!users.containsKey(msg.getSender().getNick())) {
            throw new RemoteException ("Join channel before sending messages");
        }

        // Get a snapshot of our users and send them the message.
        users.copyOfValues().forEach( usr -> {
            try {
                usr.sendMessage (msg);  
            } catch (Exception e) { } // Nothing to do. We already purged. Continue to next.
        });
    }
        
    //
    // When users leave or join this channel, we send a message to the remaning users, so that
    // they can update their fancy UI's :)
    //
    // Notification is sent only to the list of users provided as argument.
    //
    private void notifyUsers (Collection<IChatUser> usersList, String code, String nick) {
        // Build the message to send.
        IChatMessage msg;
        try {
            msg = new ChatMessageImpl (null, this, code + " " + nick);
        } catch (RemoteException e) {return;} // Failed to build. Well, theoretically.

        // send the same message to all users in the collection
        usersList.forEach( usr -> {
            try {
                usr.sendMessage (msg);
            } catch (RemoteException e) {} // Ignore errors when sending channel notifications
        });              
    }

    //
    // private function to purge stale users
    // students need not fully understand this code. 
    //
    private void purge (){
        
        List<String> staleList = new ArrayList<>();        
        
        // get a synchronized copy, to work with it.
        Map<String, IChatUser> copy = users.copyOf();
        
        copy.forEach( (nick, user) -> {        
            try {
                user.getNick(); // Remote invo to check if stale or alive
            } catch (RemoteException e) {
                staleList.add(nick); // mark as stale.
            }
        });
        
        // Update copy and original, removing stale users
        staleList.forEach( key -> {copy.remove(key); users.remove(key);});                   
        
        // Notify alive users about stale users
        staleList.forEach( key -> notifyUsers (copy.values(), LEAVE, key));                   
    }
    
    
}
