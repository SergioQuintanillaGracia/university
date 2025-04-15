//
// This file must be implemented when completing "ChatRobot activity"
//

import utils_rmi.ChatConfiguration;
import faces.IChatMessage;
import faces.MessageListener;
import java.rmi.*;
import java.rmi.registry.*;
import java.rmi.server.*;
import java.util.*;

/**
 * ChatRobot implementation
 * 
 * <p> Notice ChatRobot implements MessageListener. MUST not extend ChatClient.
 * 
 */

public class ChatRobot implements MessageListener
{

    private ChatConfiguration conf;
    public ChatRobot (ChatConfiguration conf) {
        this.conf = conf;
    }
    
   @Override
   public void messageArrived (IChatMessage msg) {
       //*****************************************************************
       // Activity: implement robot message processing
       
   }
   
   private void work () {
       
       String channelName = conf.getChannelName();
       if (channelName == null) channelName = "#Linux";
       System.out.println ("Robot will connect to server: '" + conf.getServerName() + "'" + 
               ", channel: '" + channelName + "'" + 
               ", nick: '" + conf.getNick() + "'" +        
               ", using name server: '" + conf.getNameServerHost() + ":" + conf.getNameServerPort() + "'");
       
       try {
           //*****************************************************************
           // Activity: implement robot connection and joining to channel
           
           
       } catch (Exception e) {
           System.out.println("Something went wrong: " + e);
       }
       
   }

   public static void main (String args [])  {
       ChatRobot cr = new ChatRobot (ChatConfiguration.parse (args));
       cr.work ();
   }
}
