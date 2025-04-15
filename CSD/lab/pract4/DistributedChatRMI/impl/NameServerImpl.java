package impl;

import utils.SynchroMap;
import faces.INameServer;
import java.rmi.AlreadyBoundException;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import static utils_rmi.RemoteUtils.remote2String;

/**
 * NameServer implementation. rmiregistry drop-in replacement.
 * 
 * <p>Implementation is trivial. Just a map.
 * 
 * @author Pablo Galdamez
 */

public class NameServerImpl extends UnicastRemoteObject
        implements INameServer
{
    private final SynchroMap<String, Remote> map = new SynchroMap<>();

    //
    // Simple constructor. Nothing interesting to do.    
    //
    public NameServerImpl () throws RemoteException {  }
    
    //
    // ISA INameService
    //
    @Override
    public void bind (String name, Remote obj) throws RemoteException, AlreadyBoundException
    {        
        System.out.println (" ==> bind ( " + name + ", " + remote2String(obj) + " )");
        synchronized (map) { // Sync, to make "get+put" atomic
            if (map.get(name) != null) throw new AlreadyBoundException();
            map.put(name, obj);
        }
    }
    
    //
    // ISA INameService
    //
    @Override
    public void rebind (String name, Remote obj) throws RemoteException {
        System.out.println (" ==> rebind ( " + name + ", " + remote2String(obj) + " )");
        map.put(name, obj);
    }
    
    //
    // ISA INameService
    //
    @Override
    public void unbind (String name) throws RemoteException {
        System.out.println (" ==> unbind ( " + name + " )");
        map.remove(name);
    }
    
    //
    // ISA INameService
    //
    @Override
    public Remote lookup (String name) throws RemoteException
    {
        Remote obj = map.get (name);
        System.out.println (" ==> resolve (" + name + ") --> " + remote2String (obj));
        return obj;
    }
  
    
}
