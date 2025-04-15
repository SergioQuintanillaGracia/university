//
// Students can safely ignore this file.
// It just contains some basic IP utilities.
//

package utils;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.LinkedList;
import java.util.List;
import java.util.function.Function;

//
// Students can safely ignore this file.
// It just contains some basic IP utilities.
//

/**
 * Basic IP utilities to figure out which is my own IP address.
 * 
 * @author Pablo Galdamez
 */
public class NetUtils {
    private NetUtils () {}
    
    public static void printNetworkInterfaces() {
        List<NetworkInterface> nets = getNetworkInterfaces ();
        System.out.println ("Network interfaces:");
        nets.forEach( net -> {
            
           Boolean isppp = null;
           try {
               isppp = net.isPointToPoint();
           } catch (Exception e) {}
           
           System.out.println ("IFace: " + net.getName()) ;
           System.out.println ("   displayName: " +  net.getDisplayName());
           System.out.println ("   parent: " +  net.getParent());
           System.out.println ("   isVirtual: " +  net.isVirtual());
           System.out.println ("   isPPP: " +  isppp);
           
           List<InetAddress> alist = getIPAddresses (net);
           alist.forEach( addr -> {
               printAddress (addr);
           });
           
        });
    }

    public static void printAddress (InetAddress addr) {
        System.out.println("   Address: " + addr.getHostAddress());
        System.out.println("      loop: " + addr.isLoopbackAddress());
        System.out.println("      linkLocal: " + addr.isLinkLocalAddress());
        System.out.println("      anyLocal: " + addr.isAnyLocalAddress());
        System.out.println("      siteLocal: " + addr.isSiteLocalAddress());            
    }
    
    /**
     * Get a list of network interfaces running in this computer. Select only those which are Up.
     * 
     * @return the list, maybe empty, never null.
     */
    public static List<NetworkInterface> getNetworkInterfaces() {
        List<NetworkInterface> list = new LinkedList<>();        
        Enumeration<NetworkInterface> interfaces = null;
        try {
            interfaces = NetworkInterface.getNetworkInterfaces();            
        } catch (SocketException e) { return list;}
        
        while (interfaces.hasMoreElements()) {
            NetworkInterface ni = interfaces.nextElement();
            try {
                if (ni.isUp()) list.add(ni);
            } catch (SocketException e) { } 
        }
        return list;
    }

    // Get addresses for a given network interface
    private static List<InetAddress> getIPAddresses( NetworkInterface net) {
        List<InetAddress> list = new ArrayList<InetAddress>();
        Enumeration <InetAddress> addresses = net.getInetAddresses();                
        while (addresses.hasMoreElements()) {
            InetAddress addr = addresses.nextElement();
            list.add (addr);
        }
        return list;
    }

    /**
     * Get a list of all known IP addresses for the current host,
     * according to the specified filter.
     * 
     * @param filter filters addresses (true to keep the address)
     * @return a List of <code>InetAddress</code>, may be empty but never null.
     */
    public static List<InetAddress> getIPAddresses( Function <InetAddress, Boolean> filter) {
        List<InetAddress> list = new ArrayList<InetAddress>();
        List<NetworkInterface> nets = getNetworkInterfaces ();
        nets.forEach( net -> {            
            List<InetAddress> alist = getIPAddresses(net);                
            alist.forEach( addr -> {
                if ((filter == null) || filter.apply(addr)) list.add (addr);
            });
        });
        return list;
    }

    /**
     * Get the most appropiate IP address bound to this computer.
     * 
     * If there's more than one, return the first.
     * 
     * @return IP address for this computer. 
     */
    public static InetAddress getDefaultAddress () {
        InetAddress defAddr = null;
        try {
            defAddr = InetAddress.getLocalHost();        
        } catch (UnknownHostException e) {}

        // We are not happy if default is a loopback. This happens in Linux        
        if (defAddr == null || defAddr.isLoopbackAddress()) {
            List<InetAddress> list = getIPAddresses(addr -> !(addr.isLoopbackAddress() || addr.isLinkLocalAddress()) );
            if (!list.isEmpty()) {
                defAddr = list.get(0); // take the first
            }            
        }        

        // If there's nothing approapiate, return the loopback.
        return defAddr != null ? defAddr : InetAddress.getLoopbackAddress();
    }
    
    //
    // Utility to show our network interfaces and our "calculated" default address
    //
    public static void main (String args [] ) throws Exception
    {
        printNetworkInterfaces ();    
        System.out.println ();                
        System.out.println ("Default address --> " + getDefaultAddress ().getHostAddress());
    }
}
