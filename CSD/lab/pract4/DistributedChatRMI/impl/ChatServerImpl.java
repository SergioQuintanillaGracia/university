package impl;

import utils_rmi.ChatConfiguration;
import utils.SynchroMap;
import faces.IChatChannel;
import faces.IChatUser;
import faces.IChatServer;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

/**
 * IChatServer implementation
 * 
 * <p>To understand this code, review interface IChatServer first.
 * 
 * @author Pablo Galdamez
 */

public class ChatServerImpl extends UnicastRemoteObject
        implements IChatServer 
{
    // Data structurs for the server is just a couple of Hash tables. 
    // One hash table to keep track of connected users, the other one to keep
    // track of registered channels.
    
    private final SynchroMap <String, IChatUser> users = new SynchroMap<> ();
    private final SynchroMap <String, IChatChannel> channels = new SynchroMap<> ();

    /**
     * Constructor for ChatServerImpl
     * 
     * @param conf configuration where we get the port.
     * 
     * @throws RemoteException Error
     */
    public ChatServerImpl (ChatConfiguration conf) throws RemoteException {
        super (ChatConfiguration.the().getMyPort());
    }
    
    //
    // ISA IChatServer
    //
    // We simply return a copy of the Map where we keep all channels.
    //
    @Override
    public IChatChannel [] listChannels () throws RemoteException
    {
        purgeChannels (); // remove stale channels, if any.
        synchronized (channels) {
            return channels.values().toArray(new IChatChannel[channels.size()]);
        }
    }
    
    //
    // ISA IChatServer
    //
    // Returns a named channel. If not found or stale, returns null. 
    // If stale, removes it.
    //
    @Override
    public IChatChannel getChannel (String name) throws RemoteException 
    {
        name = name.trim();
        purgeChannels ();
        IChatChannel ch = channels.get (name);
        return checkChannel (name, ch) == null ? null : ch;
    }

    //
    // ISA IChatServer
    //
    // To create a channel, first check if already exists.
    // Only create if not found.
    //
    @Override
    public IChatChannel createChannel (String name) throws RemoteException {
        name = name.trim();
        IChatChannel ch;
        synchronized (channels) {
            if (getChannel (name) != null) return null; // Channel with same name exists
            ch = new ChatChannelImpl (name);
            channels.put (name, ch);
        }
        System.out.println ("Channel '" + name + "' created.");
        return ch; // Success
    }
    
    //
    // ISA IChatServer
    //
    // Returns a named user. 
    // If not found or stale, returns null. If stale, removes it.
    //
    @Override
    public IChatUser getUser (String nick) throws RemoteException
    {
        nick = nick.trim();
        IChatUser user = users.get (nick);
        String foundAs = checkUser (nick, user);

        // It's common get a reference to some ChatUser, to chat with her. If user is stale, we better tell 
        // channels about the situation, so that channels remove stale users too.
        if (foundAs == null) kickUserFromChannels (user);

        return foundAs == null ? null : user;
    }
    
    //
    // ISA IChatServer
    //
    // Connect means to check if user already exists. If it doesn't exist, allow connection.
    // If it didn't exist, proceed with connection, which is just to store user in 
    // the server hash table.
    //
    @Override
    public void connectUser (IChatUser usr) throws RemoteException
    {
        // Remove users, to allow new user to use same nick as stale users.
        purgeUsers(); 
        
        // If new user is stale, don't connect.
        String nick = checkUser (null, usr);
        if (nick == null) throw new RemoteException ("Stale user");
    
        synchronized (users) {
            // If user already exists, don't connect.
            if (users.get (nick) != null) throw new RemoteException ("User exists"); 
            
            // Everything was ok.
            users.put (nick, usr);      
        }
        System.out.println ("User '" + nick + "' connected.");
    }
    
    //
    // ISA IChatServer
    // 
    // Disconnect means to remove the user from our hash table, and to command 
    // channels to remove this user too.
    //
    @Override
    public void disconnectUser (IChatUser usr) throws RemoteException
    {
        // Avoid rt error, which shouldn't happen actually.
        if (usr == null) throw new RemoteException ("null user");
        
        // If stale, don't continue
        String nick = checkUser (null, usr);
        if (nick == null) throw new RemoteException ("Stale user");
        
        // If not found, don't do anything else. If found, remove it.
        synchronized (users) {
            IChatUser found = users.get (nick);
            if (found == null) throw new RemoteException ("Already disconnected");
            if (!found.equals (usr)) throw new RemoteException ("Cannot impersonate");
        
            // User found and it was the same as the argument, then remove it from hash table
            users.remove (nick, usr);
        }
        System.out.println ("User '" + nick + "' disconnected.");
        
        // Help channels to keep updated, removing user too.
        kickUserFromChannels (usr);
    }
    
    //
    // Utility function to ckeck if a channel is stale or alive.
    //
    // Returns null if channel is stale. Returns name of channel if alive.
    //
    private String checkChannel (String name, IChatChannel ch) 
    {
        if (ch == null) return null; 
        
        // Check if stale. We check by calling remotely.
        String reply;
        try {
            reply = ch.getName(); // Remote call!!
        } catch (RemoteException e) {                
            System.out.println ("Removing stale channel: " + name);
            channels.remove (name, ch);
            return null; // Channel is stale.
        }
        return reply; // Channel is ok,
    }
    
    //
    // Utility function to ckeck if a user is stale or alive.
    //
    // Returns null if user is stale. Returns user's nick if alive.
    //
    private String checkUser (String nick, IChatUser user) 
    {
        if (user == null) return null;
        
        // Check if stale, by calling remotely.
        String reply;
        try {
            reply = user.getNick(); // Remote call!!
        } catch (RemoteException e) {                
            if (nick != null) {
                System.out.println ("Removing stale user: " + nick);
                users.remove (nick, user);
            }
            return null; // User is stale.
        }
        return reply; // User is ok,
    }

    //
    // Utility function to remove stale channels
    //
    private void purgeChannels (){        
        channels.copyOf().forEach( this::checkChannel ); // Lambda magic :)
    }
    
    //
    // Utility function to remove stale users
    //
    private void purgeUsers (){        
        users.copyOf().forEach( this::checkUser ); // Lambda magic
        kickUserFromChannels (null); // purge stale users from all channels
    }

    //
    // Utility function to kick a specific user from all channels.
    //
    private void kickUserFromChannels (IChatUser user) {
        if (user == null) return;
        channels.copyOfValues().forEach( chan -> {
            try {
                chan.leave(user);
            } catch (Exception e) { }
        });
    }
    
}
