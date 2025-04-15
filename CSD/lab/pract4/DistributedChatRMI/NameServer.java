import impl.NameServerImpl;
import utils_rmi.ChatConfiguration;
import faces.INameServer;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

/**
 * NameServer process. rmiregistry drop-in replacement.
 * 
 * @author Pablo Galdamez
 */

public class NameServer
{  

    /**
     * Main NameServer code. Creates an instance and binds it to our own local RMI 
     * Registry. Other process can call our registry to get a reference to us, the 
     * NameServer.
     * 
     * @param args command line parameters
     */

    public static void main (String [] args) {
        try {
            // Parse configuration to get our port number
            //
            ChatConfiguration conf = ChatConfiguration.parse(args);
            
            // Create the NameServerImpl remote object.
            //
            INameServer ns = new NameServerImpl ();
            
            // Create a local RMI registry in this process.
            // This registry will listen at the specified port, in this computer's IP address.
            //
            Registry reg = LocateRegistry.createRegistry (conf.getMyPort());
            
            // Install our NameServer into RMI local Registry.
            //
            reg.rebind (INameServer.NS_NAME, ns);
            
            // Success !!
            //
            System.out.println ("OK ==> 'NameServer' running at " + conf.getMyHost() + ":" + conf.getMyPort() );
            System.out.println ("I'm good boy, don't kill me please! ");
            
        } catch (RemoteException e) {
            Throwable t = e.getCause();
            if (t != null && t instanceof java.net.BindException) {
                System.err.println("Error. port " + ChatConfiguration.the().getMyPort() + " already in use. ");
                System.err.println("Please specify different port using parameter 'port' ");                
            } else {
                System.err.println("Error creating name server: " + e);
            }
            System.exit(-1);
        }
    }   
}
