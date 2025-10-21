import impl.ChatServerImpl;
import utils_rmi.ChatConfiguration;
import faces.INameServer;
import faces.IChatServer;
import java.rmi.RemoteException;

/**
 * This is the ChatServer process.
 * 
 * <p>The ChatServer process creates a ChatServerImpl object instance and creates 
 * some default channels inside it.
 * 
 * <p>After creation, it binds the ChatServerImpl object to the name server and
 * waits invocations forever.
 * 
 * @author Pablo Galdamez
 */

public class ChatServer
{
    
    private final ChatConfiguration conf;
    
    public ChatServer (ChatConfiguration conf) {
        this.conf = conf;
    }
    
    //
    // This is the main startup routine
    //
    private void startup () {

        // Create the ChatServerImpl Remote object
        //
        IChatServer cs = null;        
        try {
            cs= new ChatServerImpl (conf);
        } catch (Exception e) {
            System.out.println ("Failed to create ChatServerImpl: " + e);
            System.exit(-1);            
        }

        // Create default channels inside ChatServerImpl
        //
        try {
            cs.createChannel ("#Spain");
            cs.createChannel ("#Linux");
            cs.createChannel ("#Friends");
        } catch (RemoteException e) {
            System.out.println ("Failed to create channels: " + e);
            System.exit(-1);
        }
        
        // Register ChatServerImpl at rmiregistry
        /*
        try {   
            Registry reg = LocateRegistry.getRegistry (conf.getNameServerHost(), conf.getNameServerPort() );            
            reg.rebind (conf.getServerName(), cs);
        } catch (java.rmi.ConnectException e) {
            System.out.println ("rmiregistry not found at '" + 
                    conf.getNameServerHost() + ":" + conf.getNameServerPort() + "'");
            System.exit(-1);
        } catch (Exception e) {
            System.out.println("Error connecting to rmiregistry: " + e); 
            System.exit(-1);
        }*/

        // Register ChatServerImpl at our name server
        //
        try {
            INameServer ns = INameServer.getNameServer(conf.getNameServerHost(), conf.getNameServerPort());
            ns.rebind (conf.getServerName(), cs);
        } catch (Exception e) {
            System.out.println("Error connecting to NameServer: " + e); 
            System.exit(-1);            
        }
        
        // Registered object "ChatServerImpl", prevents us from exiting.
        // Server runs "forever".
        //
        System.out.println("OK ==> '" + conf.getServerName() + "' Running at " + 
                conf.getMyHost() + ":" + conf.getMyPort());
    }
    
    public static void main (String args [] ) throws Exception
    {
        // Create ChatServer, not ChatServerImpl
        //
        ChatServer cs = new ChatServer (ChatConfiguration.parse (args));
        
        cs.startup ();
    }
}
