package faces;

import java.rmi.AlreadyBoundException;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;


/**
 * Name Service interface "clone" of rmiregistry.
 * 
 * @author Pablo Galdamez
 */

public interface INameServer extends Remote {
    
    static String NS_NAME = "ns"; 
   
    /**
     * Static method to easily get a reference to our NameServer.
     * 
     * <p>Every RMI process has a "Registry". Our NameServer process has a local Registry too.
     * The NameServer process creates a local object, called NameServerImpl and registers it
     * into its local Registry.
     *
     * <p>By RMI restrictions, decided by RMI engineers, the local Registry at each process can only 
     * store references to local objects, so we stored a reference to the local NameServerImpl object. 
     * However, the NameServerImpl itself is a fully-fledged object that can store references to other 
     * objects without restrictions.
     * 
     * <p>We just need to get a reference to the NameServer to let the ball start rolling.
     * 
     * @param host host where the NameServer is running
     * @param port port where the NameServerImpl object is bound.
     * 
     * @return a reference to the NameServerImpl
     * 
     * @throws Exception Error getting the NameServer reference
     * 
     */

    public static INameServer getNameServer (String host, int port) throws Exception {
        Registry reg = LocateRegistry.getRegistry (host, port);
        return (INameServer) reg.lookup (NS_NAME);      
    }
       
    /**
     * Tries to bind an object to a name. 
     * 
     * <p>If name was already present, throws an "AlreadyBoundException"
     * 
     * @param name name to bind the object with
     * @param obj object to be bound
     * 
     * @throws RemoteException Error
     * @throws AlreadyBoundException Name already bound 
     */
    public void bind (String name, Remote obj) throws RemoteException,
            AlreadyBoundException;


    /**
     * Rebind a name to an object.
     * 
     * <p>It's similar to sequentially invoke unbind() followed by bind().
     * 
     * @param name name to bind the object with
     * @param obj object to be bound
     * 
     * @throws RemoteException Error rebinding
     */    
    public void rebind (String name, Remote obj) throws RemoteException;
    
    
    /**
     * Remove a binding made to the given name.
     * 
     * <p>If not found, this method does nothing and does not throw any exception
     * 
     * @param name name to unbind
     * 
     * @throws RemoteException Error unbinding
     */
    public void unbind (String name) throws RemoteException;

    
    /**
     * Returns the bound object or null of not found.
     * 
     * @param name name to search
     * 
     * @return object bound to the name, or null of not found.
     * 
     * @throws RemoteException Error
     */    
    public Remote lookup (String name) throws RemoteException;
}
