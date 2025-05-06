//
// This file must be implemented when completing "ChatRobot activity"
//

import faces.IChatMessage;
import faces.IChatServer;
import faces.INameServer;
import faces.MessageListener;
import java.rmi.*;
import java.rmi.registry.*;
import java.rmi.server.*;
import java.util.*;
import utils_rmi.ChatConfiguration;
import utils_rmi.RemoteUtils;

/**
 * ChatRobot implementation
 *
 * <p> Notice ChatRobot implements MessageListener. MUST not extend ChatClient.
 *
 */

public class ChatRobot implements MessageListener {

    private ChatConfiguration conf;

    public ChatRobot(ChatConfiguration conf) {
        this.conf = conf;
    }

    @Override
    public void messageArrived(IChatMessage msg) {
        //*****************************************************************
        // Activity: implement robot message processing

    }

    private void work() {
        String channelName = conf.getChannelName();
        if (channelName == null) channelName = "#Linux";
        System.out.println(
            "Robot will connect to server: '" +
            conf.getServerName() +
            "'" +
            ", channel: '" +
            channelName +
            "'" +
            ", nick: '" +
            conf.getNick() +
            "'" +
            ", using name server: '" +
            conf.getNameServerHost() +
            ":" +
            conf.getNameServerPort() +
            "'"
        );

        try {
            //*****************************************************************
            // Activity: implement robot connection and joining to channel
            String nshost = conf.getNameServerHost();
            int nsport = conf.getNameServerPort();
            String serverName = conf.getServerName();
            String nick = conf.getNick();
            INameServer ns = INameServer.getNameServer(nshost, nsport);
            System.out.println(RemoteUtils.remote2String(ns));

            IChatServer chatServer = (IChatServer) ns.lookup(serverName);

            ChatUserImpl user = new ChatUserImpl(nick, messageArrived);
        } catch (Exception e) {
            System.out.println("Something went wrong: " + e);
        }
    }

    public static void main(String args[]) {
        ChatRobot cr = new ChatRobot(ChatConfiguration.parse(args));
        cr.work();
    }
}
