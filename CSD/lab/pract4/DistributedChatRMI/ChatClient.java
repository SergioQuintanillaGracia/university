import ui.ChatUI;
import ui.ChatClientFace;
import ui.ChatUIFace;
import faces.MessageListener;
import impl.ChatUserImpl;
import impl.ChatMessageImpl;
import impl.ChatChannelImpl;
import utils_rmi.ChatConfiguration;
import faces.IChatMessage;
import faces.IChatChannel;
import faces.IChatUser;
import faces.INameServer;
import faces.IChatServer;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

/**
 * Main RMI Client process.
 * 
 * <p> This process creates a chat user interface. The user interface calls this class public methods
 * as specified by interface ChatClientFace.java.
 * 
 * <p>Relevant methods that user interface calls, are those to connect to some chat server, 
 * to retrieve its channels, to disconnect, to send a message, etc. All methods are listed
 * into ChatClientFace. It's advisable to review that interface before studying this class.
 * 
 * <p> This class also contains a basic message listener for one chat user: our user.
 * All messages sent to our user, will be routed to us, as "MessageListener".
 * 
 * @author Pablo Galdamez
 */

public final class ChatClient
        implements ChatClientFace, MessageListener
{
    private final ChatConfiguration conf; // Chat system parameters
    private IChatServer srv = null;   // We just connect to one single server
    private IChatUser myUser = null;  // Our own user    
    private ChatUIFace ui = null;     // User interface for fancy UI-driven chatting.
    
    /**
     * Constructor just needs a chat configuration.   
     * 
     * @param conf the configurable parameters
     */
    public ChatClient (ChatConfiguration conf) {
        this.conf = conf;
    }
    
    //
    // Install UI
    //
    private void setUI (ChatUIFace ui) {this.ui = ui;}
    
    //
    // ISA ChatClientFace
    // 

    /**
     * Connect to the ChatServerRMI
     * 
     * <p> Connect means to do several steps. First, find a IChatServer instance from the NameServer. 
     * second, create a new user to represent us, and register this user into the server, and last, to
     * retrieve the server channel list.
     * 
     * @param serverName Server name to lookup into the NameServer
     * @param nick my own nick name
     * 
     * @return list of channels created at the server
     * @throws Exception Error
     */
    @Override
    public String [] doConnect (String serverName, String nick) throws Exception {
        
        // Locate server using the name service
        try {
           
            //
            // ** Obtain a reference to the name server, using LocateRegistry.getRegistry. 
            // Store this reference in a variable of type "Registry".  
            // 
            // Alternatively ( we chose this option )
            // ** Obtain a reference to the name server, using INameServer.getNameServer
            // Store this reference in a variable of type "INameServer".  
            
            
            // Registry reg = LocateRegistry.getRegistry (conf.getNameServerHost(), conf.getNameServerPort() );

            INameServer reg = INameServer.getNameServer(conf.getNameServerHost(), conf.getNameServerPort());           
            
            // Look for the object "IChatServer" in the name server, using the previous reference. 
            // Remember that the object name is stored in the variable "serverName" (input parameter of the "doConnect" method) 
            // Store the obtained remote reference in the variable "srv" (defined at the beginning of ChatClient class)
            
            srv = (IChatServer) reg.lookup (serverName);
                        
        } catch (java.rmi.ConnectException e) {
            throw new Exception ("Name server not found at '" + 
                    conf.getNameServerHost() + ":" + conf.getNameServerPort() + "'");
        } catch (Exception e) {
            throw new Exception ("Error connecting to name server: " + e); 
        }

        // if we didn't find it, raise exception
        if (srv == null) throw new Exception ("Server '" + serverName + "' not found");

        
        // Once we've got the server, we create a local user object and connect the user to the server     
        
        //
        // Create the ChatUser object, indicating as parameters the nick of the user and "this" as MessageListener. 
        //
        myUser = new ChatUserImpl (nick, this);
        
        
        //
        // Connect the client to the ChatServer.
        //
        srv.connectUser (myUser);
        IChatChannel [] channels=null;  
        
        //
        // Obtain the list of channels. Keep the list of channels at variable "channels"
        //
        channels = srv.listChannels();
        
        //
        // Check if we got any channels at all
        //
        if (channels == null || channels.length == 0) 
            throw new Exception ("Server has no channels");
        
        //
        // Convert channel list to string list, since we don't want the UI to know about invocable
        // objects. It is a plain UI which does not depend on RMI, 
        //
        String list [] = new String [channels.length];      
        for (int i=0; i<channels.length; i++) {
            list[i] = channels[i].getName();
        }
        
        // Connected to server. Things went fine :)
        return list;
    }
    
    //
    // ISA ChatClientFace.
    //

    /**
     * 
     * This method is invoked by UI to disconnect from server .
     */
    
    @Override
    public void doDisconnect () {
        try {
            if (srv != null && myUser != null) srv.disconnectUser(myUser);
        } catch (RemoteException e){}
    }
    
    //
    // ISA ChatClientFace.
    //
    
    /**
     * UI commands to leave a specific channel.
     * 
     * @param channelName channel name
     * @throws Exception Error
     */
    @Override
    public void doLeaveChannel (String channelName) throws Exception {
        IChatChannel ch = srv.getChannel (channelName);
        if (ch != null) {
            
            // Make user "myUser" to leave channel "ch".   
            //
            ch.leave(myUser);
            
        }
    }
    
    //
    // ISA ChatClientFace
    //
    
    /**
     * Join a channel, specified as parameter.
     * 
     * <p>On success return the chanel users list as a list of user nick names.
     * 
     * @param channelName channel to join
     * 
     * @return list of users already connected to same channel
     * @throws Exception Errors
     */
    @Override
    public String [] doJoinChannel (String channelName) throws Exception {

        // Get a reference to the channel with the specified channel name.        
        IChatChannel ch = srv.getChannel (channelName);
        if (ch == null) {throw new Exception ("Channel not found");}
                
        IChatUser [] users;
        
        // Join channel, and get it's users.
        //
        users = ch.join (myUser);
        
        //
        // Check users
        //
        if (users == null || users.length == 0) 
            throw new Exception ("No users. This shouldn't happen once we join.");
        
        // 
        // Convert user list into string list. To make UI unaware of RMI stuff
        //
        String [] userList = new String [users.length];      
        for (int i=0; i<users.length; i++) {
            userList[i] = users[i].getNick();
        }
        
        return userList;
    }
    
    //
    // ISA ChatClientFace
    //
    
    /**
     * 
     * UI wants to send a message to a channel... lets do it creating a IChatMessage
     * 
     * @param dst Destination channel
     * @param msg message to send
     * 
     * @throws Exception Errors
     */
    @Override
    public void doSendChannelMessage (String dst, String msg) throws Exception
    {
        try {
            IChatChannel c_dst = srv.getChannel (dst);
            IChatMessage c_msg = new ChatMessageImpl(myUser, c_dst, msg);
            
            // Send the message to the destinationchannel. 
            //
            c_dst.sendMessage(c_msg);
            
        } catch (RemoteException e) {
            throw new Exception ("Cannot send message", e);
        }
    }

    // 
    // ISA ChatClientFace
    //
    
    /**
     * UI wants to send a private message to some user... lets do it creating a ChatMessageImpl.
     * ChatMessageImpl is a fully-fledged remote object that resides at this RMI process.
     * 
     * @param dst Nick name message destination
     * @param msg message to send
     * 
     * @throws Exception Errors
     */

    @Override
    public void doSendPrivateMessage (String dst, String msg) throws Exception
    {
        try {
            // From server, get the user with the specified nick
            IChatUser u_dst = srv.getUser (dst);
            
            // Create a ChatMessageImpl object.
            IChatMessage c_msg = new ChatMessageImpl(myUser, u_dst, msg);
            if (u_dst == null) throw new Exception ("User '" + dst + "' disconnected");
            
            // Send the message to the destination user  
            //	  
            u_dst.sendMessage(c_msg);
            
        } catch (Exception e) {
            throw new Exception ("Cannot send message", e);
        }
        
    }
    
    //
    // ISA ChatClientFace
    //
    
    /**
     * 
     * Terminate this program.
     */
    @Override
    public void doTerminate () {
        doDisconnect ();
        System.exit (0);
    }
    
    //
    // ISA MessageListener
    //
    
    /**
     * MessageArrived
     * 
     * <p>This is one of the most important pieces of code to understand how the chat
     * system works. Some message came from a channel or from a remote user. 
     * We need to process it here.
     * 
     * <p>Messages from the server can be sent by some user (user messages) or sent
     * by the channel itself (control messages).
     * 
     * <p>User messages can be sent to us (private message) or to some channel
     * (public message)
     * 
     * <p>Control messages sent by the channel are notifications about JOIN/LEAVE.
     * These notifications allows us to know precisely which users are connected to
     * some channel timely.
     * 
     * @param msg message that comes from server
     */

    @Override
    public void messageArrived (IChatMessage msg) {
        try {
            // Get the message sender
            //
            IChatUser src = msg.getSender();
            
            // Get the message destination: it can be IChatUser or IChatChannel.
            //
            Remote dst = msg.getDestination();
            
            // Get the message contents.
            //
            String str = msg.getText();

            // if (isPrivate() is true) this is a user message: private message 
            // sent to me            
            //
            if (msg.isPrivate()) {
                IChatUser u_dst = (IChatUser) dst;
                ui.showPrivateMessage (src.getNick(), u_dst.getNick(), str);
                return;   // done!!
            }

            // Now we know destination is a channel. Get the destination channel.            
            //
            IChatChannel c_dst = (IChatChannel) dst;
            
            // Messages sent to a channel, have some "source": the user who sent it.
            //
            // If "src != null" I know the message was sent by someone to one of the 
            // channels I have joined.
            //
            if (src != null) { 
                ui.showChannelMessage (src.getNick(), c_dst.getName(), str);
                return;  // done !!
            }

            //
            // Control messages have no "source".            
            // Now i'm sure this is a control message. "src == null".            
            //
            String nick;
            if (str.startsWith (ChatChannelImpl.LEAVE)) {

                //
                // Control message that informs about some user who left the channel
                //
                nick = str.substring (ChatChannelImpl.LEAVE.length() + 1);
                ui.showUserLeavesChannel (c_dst.getName(), nick);
                
            } else if (str.startsWith (ChatChannelImpl.JOIN)) {

                //
                // Control message that informs about some user who joins the channel
                //
                nick = str.substring (ChatChannelImpl.JOIN.length() + 1);
                ui.showUserEntersChannel (c_dst.getName(), nick);

            }
        
        } catch (RemoteException e) {
            ui.showErrorMessage ("MessageArrived failed --> " + e);
        }
    }

    
    /**
     * Main ChatClientRMI program
     * 
     * <p>It creates the Client object, and shows the graphical interface.
     * 
     * <p>Everything that happens from now on will be commanded by the graphical interface.
     * 
     * <p>Graphical interface will call our methods as speficied at ChatClientFace.java
     * @param args command line parameters
     */

    public static void main (String [] args) {
        ChatClient cc = new ChatClient (ChatConfiguration.parse (args));
        ChatUI ui = new ChatUI (cc, "Chat Client RMI", cc.conf.getServerName(), cc.conf.getNick(), cc.conf.getChannelName());
        cc.setUI (ui);
        ui.show();        
        System.out.println("OK ==> 'ChatClient' running at " + cc.conf.getMyHost() + ":" + cc.conf.getMyPort());        
    }
    
}
