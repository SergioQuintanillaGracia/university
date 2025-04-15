//
// Students need not modify nor fully understand this file.
// The single important aspect about this Class is how to use it, not how it works.
// You can learn how to use it, reading public methods' documentation.
//

package utils_rmi;

import java.rmi.Remote;
import java.rmi.server.UnicastRemoteObject;
import utils.NetUtils;
import utils.ProgramParameters;

//
// Students need not modify nor fully understand this file.
// The single important aspect about this Class is how to use it, not how it works.
//

/**
 * ChatConfiguration object.
 * 
 * <p>important methods are exposed as public "get_XXX" methods to get configurable parameters.
 * 
 * <p>This file provides a basic and simple command line parsing to configure our chat programs.
 * 
 * <p>A ChatConfiguration is always ready to be used as a singleton object. This object must be 
 * created at early stages. You can see ChatServer.java or ChatClient.java main() to realize where
 * and how to create this configuration object.
 * 
 * @author Pablo Galdamez
 */

public class ChatConfiguration {
    
    private static final int NS_PORT = 9000;

    //
    // Configuration info for the "ProgramParameters" utility where we specify parameters   
    //
    private final String [][] pp_parameters = {
        {"nsHost", "<host where name server is running>", "localhost"},
        {"nsPort", "<port where name server is listening>", NS_PORT + ""},
        {"serverName", "<ChatServer object name to bind/lookup into NameServer>", "TestServer"},
        {"host", "<Host name or address to be included into my object references>"},
        {"port", "<Port number where to listen for connections>", NS_PORT + ""},  // only for NameServer
        {"nick", "<User name to use when connecting to a ChatServer>"},
        {"channel", "<Channel to auto-join when program starts>"}
    };
    
    //
    // Configuration info for the "ProgramParameters" utility where we specify programs
    //
    private final String [][] pp_programs = {
        {"ChatServer", "nsHost", "nsPort", "serverName", "host"},
        {"ChatClient", "nsHost", "nsPort", "serverName", "host", "nick", "channel"},
        {"ChatRobot", "nsHost", "nsPort", "serverName", "host", "nick", "channel"},
        {"NameServer", "host", "port"}       
    };
    
    //
    // A ProgramParameters instance tailor to our programs.
    //
    private final ProgramParameters parameters = new ProgramParameters (pp_parameters, pp_programs)
            .addChecker ("nsPort", Integer::parseUnsignedInt)
            .addChecker ("port", Integer::parseUnsignedInt)
            .addExample ("ChatClient", "")
            .addExample ("ChatClient", "nsport=9010")
            .addExample ("ChatClient", "nshost=ldsic-vdi03  nsport=9010 nick=pau", "-cp lib/*:.:")
            .addExample ("ChatRobot", "nshost=ldsic-vdi03  nsport=9010 channel=#Linux")
            .addExample ("ChatRobot", "nshost=ldsic-vdi02  nsport=9310 nick=myrobot channel=#Linux")
            .addExample ("ChatServer", "nshost=localhost  nsport=9010  serverName=test")
            .addExample ("ChatServer", "nshost=ldsic-vdi03  nsport=9010  serverName=test")
            .addExample ("NameServer", "port=10000");
    
    //
    // Port number for our RMI programs
    // We will try ports until we find one free port.
    //
    private int myPort = -1; 
    
    // 
    // Hostname or IP address to be included into our object references.
    // If user didn't specify "myHost" as parameter we will try to figure out, guessing our own IP address.
    //
    private String myHost; 
    
    
    //
    // Singleton pattern
    //
    private static ChatConfiguration the_ = null;
    
    //
    // ChatConfiguration makes use of "ProgramParameters" to parse command line.
    // Just matters that we can use the paramaters' "get" method.
    //
    private ChatConfiguration (String [] args) {
        parameters.parse (args);
        the_ = this;
    }      
    
    /**
     * Get the singleton ChatConfiguration object
     * 
     * @return the ChatConfiguration
     */
    public static ChatConfiguration the () {return the_;}

    
    /**
     * Create a ChatConfiguration from command line parameters
     * 
     * @param args vector of strings
     * 
     * @return new chatconfiguration object.
     */
    public static ChatConfiguration parse (String [] args) {

        // Create the chat configuration object, which in turn creates the ProgramParameters instance
        // and parses the command line searching parameters.
        //
        ChatConfiguration conf = new ChatConfiguration (args);
        
        //
        // Next line of code is a "hack"!
        //
        // RMI at most Linux distributions does not set up properly own host to be included 
        // into object references. It simply includes "localhost" !! This is wrong!! 
        // It seems that windows JVM works this out pretty well though. 
        // Confussion on this detail comes from Linux /etc/hosts, and hostname, bound to 127.0.1.1, 
        // at many Linux distributions.
        //
        // As a workaround we must provide the system property that RMI needs.
        // This detail is hidden into RMI's documentation stuff.
        //
        // On the other hand, since our programs always create a ChatConfiguration before anything else, 
        // it seems that "here" is nice place to set up this annoying "hack".
        //
        System.setProperty("java.rmi.server.hostname", conf.getMyHost());
        
        return conf;
    }
    
    /**
     * Get the name server host. 
     * <p> if not configured it will be "localhost"
     * 
     * @return the name server host.
     */
    public String getNameServerHost () {
        return parameters.get ("nsHost");
    }
    
   
    /**
     * Get the NameServer port.
     * <p> If not configured, default is 9000
     * 
     * @return the port where NameServer listens
     */
    public int getNameServerPort () {
        return Integer.parseUnsignedInt(parameters.get("nsPort"));
    }
   
   
   /**
    * Get the server name for the chat system.
    * 
    * <p> Use this name to locate a ChatServer into the NameServer.
    * 
    * <p> If not configured, it will be "TestServer".
    * 
    * @return the server name.
    */
   public String getServerName () {
       return parameters.get("serverName");
   }

   
   /**
    * Get own RMI process port.
    * 
    * <p> All RMI proceses are bound to some port. This allows current RMI process
    * to create RMI remote objects allowing other processes to invoke our objects.
    * 
    * <p> In essence every RMI process is a server.
    * 
    * <p> If not confiugred, port will be the first available port starting by NS_PORT+1.
    * 
    * @return own port for incomming invocactions.
    */
   public int getMyPort () {
       if (parameters.isProgram("NameServer")){
           // for name server, own port is fixed or configurable by command line.
           myPort = Integer.parseUnsignedInt(parameters.get("port"));
       } else {
           if (myPort == -1) myPort = NS_PORT+1;
           myPort = tryPorts (myPort, 10000);
       }
       return myPort;
   }

   
   /**
    * Get default channel to auto-join.
    * 
    * <p>Return null if no channel was configured.
    * 
    * <p>Some programs might join automatically to this channel.
    * 
    * @return channel to join if configured. Null otherwise.
    */
   public String getChannelName() {
       return parameters.get("channel");
   }

   
   /**
    * Get nick name
    * 
    * <p>Return null if no nick was configured.
    * 
    * @return nick name to use in chat programs. Null if not specified.
    */
   public String getNick() {
       String nick = parameters.get("nick");
       if (parameters.isProgram("ChatRobot") && nick == null) nick = "robot";              
       return nick;
   }

   
   /**
    * Get my own IP address as it will be included into own object references.
    * 
    * <p> Since any RMI process can be a server of objects, those object 
    * references will contain own IP address as location. We might need to 
    * manually specify which is our own IP address if our computer has several
    * IP addresses.
    * 
    * @return IP address for current process' object references.
    */
   public String getMyHost () {
       // If user didn't specify a host, try to figure out
       if (myHost == null) myHost = parameters.get("host");
       if (myHost == null) myHost = NetUtils.getDefaultAddress().getHostName();
       return myHost;
   }
   
   /**
    * Try ports until we find a free port, or we reach the maxPort.
    * 
    * @param port starting port number
    * @param maxPort max port number to try
    * @return first free port number.
    */
   
   private static Remote rem; // Keek this small object alive,to keep our port.
   
   private int tryPorts (int port, int maxPort) {
       while (port < maxPort) {
           try {
               rem = new UnicastRemoteObject (port) {};
               //UnicastRemoteObject.unexportObject(rem, true); Don't unexport, to keep port.
               break; // found;
           } catch (Exception e) {
               port++;
           }
       }
       try { Thread.sleep(200); } catch (Exception e) {} // Time for the OS to free up my own port.
       if (port >= maxPort) {
           System.out.println ("Wheewwww, no available IP ports! "); System.exit(-1);
       }
       return port;
   }                  
   
}
