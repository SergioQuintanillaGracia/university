//
// This file must be implemented when completing "ChatRobot activity"
//

import faces.IChatChannel;
import faces.IChatMessage;
import faces.IChatServer;
import faces.IChatUser;
import faces.INameServer;
import faces.MessageListener;
import impl.ChatMessageImpl;
import impl.ChatUserImpl;
import java.rmi.Remote;
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

        try {
            // Robot user
            ChatUserImpl user = new ChatUserImpl(conf.getNick(), this);

            String text = msg.getText();
            IChatUser sender = msg.getSender();
            Remote dest = msg.getDestination();

            System.out.printf("Received message with text: %s  |  Sender: %s\n",
                    text, sender.getNick());

            if (dest instanceof IChatChannel) {
                System.out.println("Message is public");

                // Greet new user
                if (text.startsWith("JOIN")) {
                    IChatChannel channel = (IChatChannel) dest;
                    channel.sendMessage(new ChatMessageImpl(user, channel, "Welcome!"));
                }

            } else {
                System.out.println("Message is private");

                // Answer private message
                sender.sendMessage(new ChatMessageImpl(user, sender, "I know nothing"));
            }

        } catch (Exception e) {

        }
    }

    private void work() {
        String channelName = conf.getChannelName();
        if (channelName == null) channelName = "#Linux";

        String nick = conf.getNick();

        System.out.println(
            "Robot will connect to server: '" +
            conf.getServerName() +
            "'" +
            ", channel: '" +
            channelName +
            "'" +
            ", nick: '" +
            nick +
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
            INameServer ns = INameServer.getNameServer(nshost, nsport);
            System.out.println(RemoteUtils.remote2String(ns));

            IChatServer chatServer = (IChatServer) ns.lookup(serverName);
            ChatUserImpl user = new ChatUserImpl(nick, this);

            chatServer.connectUser(user);

            IChatChannel[] channelList = chatServer.listChannels();

            // Print channel list
            System.out.print("Channel list: ");
            for (IChatChannel c : channelList) {
                System.out.print(c.getName() + ", ");
            }

            // Check if channelList contains channelName
            boolean containsChannel = false;
            for (IChatChannel c : channelList) {
                if (c.getName().equals(channelName)) {
                    containsChannel = true;
                    break;
                }
            }

            if (!containsChannel) {
                throw new Exception("Chanel does not exist");
            }

            // Join the channel
            IChatChannel channel = chatServer.getChannel(channelName);
            IChatUser[] userList = channel.join(user);

            // Create and send a message
            ChatMessageImpl message = new ChatMessageImpl(user, channel, "Among us potion guys!!1!1!");
            channel.sendMessage(message);

            // Send a private message to the second user
            userList[1].sendMessage(new ChatMessageImpl(user, userList[1], "Hi"));
            System.out.printf("Sent message to %s\n", userList[1].getNick());

        } catch (Exception e) {
            System.out.println("Something went wrong: " + e);
        }
    }

    public static void main(String args[]) {
        ChatRobot cr = new ChatRobot(ChatConfiguration.parse(args));
        cr.work();
    }
}
