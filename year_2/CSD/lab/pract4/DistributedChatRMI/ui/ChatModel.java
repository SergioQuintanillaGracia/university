//
// Students need not modify nor fully understand this file.
//

package ui;

import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import javax.swing.DefaultListModel;
import ui.SwingUtils.MyListModel;
import ui.SwingUtils.MyObservable;

//
// Students need not modify nor fully understand this file.
//

/**
 * 
 * Data model for ChatUI
 * 
 * <p> It keeps all data needed to represent a chat session (in the UI) with a chat server.
 * 
 * @author Pablo Galdamez
 */
public class ChatModel {

    // This chat session owner
    private final MyObservable<String> nick;
    
    // Server name (or url). Whereever we connect.
    private final MyObservable<String> server;
    
    // Current destination: user or channel
    private final MyObservable<ConvoModel> currentConvo;
    
    // Channel list
    private final Map<String, ChannelModel> channels = new LinkedHashMap<> ();
    
    // List of private conversation to other chatmates.
    private final Map<String, ConvoModel> privateConvos = new HashMap<> ();
    
    // Whether we have already connected or disconnected.
    private final MyObservable <Boolean> isConnected;
    
    // Simple empty channel, to ensure there's always current data.
    private final ChannelModel emptyChannel = new ChannelModel (""); // trick for empty content
    
    // Current channel.This is completely private.
    // Model offers current channel, by currentConvo.getChannel()
    private ChannelModel currentChannel;
    
    // List models as required by swing JList. 
    // Sad to say, swing forces us to have this kind of "list models".
    private final MyListModel<String> logListModel = new MyListModel<>(); // list of chat logs
    private final DefaultListModel<ConvoModel> channelListModel = new DefaultListModel<>(); // list of channels
    private final MyListModel<String> chatLineListModel = new MyListModel<>(); // list of chat lines
    private final DefaultListModel<ConvoModel> userListModel = new DefaultListModel<>(); // list of users

    
    /**
     * Create a chat model.
     * 
     * <p> Model will start not connected, with an empty chatChannel as current channel and 
     * this empty channel convo as current convo.
     * 
     */    
    public ChatModel () {
        this.isConnected = new MyObservable<>(false);
        this.nick = new MyObservable<>("");
        this.server = new MyObservable<>("");
        this.currentChannel = emptyChannel;
        this.currentConvo = new MyObservable<>(currentChannel.getChannelConvo());
        
        // on disconnect clear all models, except log and nick.
        this.isConnected.onChanged(conn -> {
           if (!conn) { 
               channelListModel.clear();
               channels.clear();
               privateConvos.clear();
               setCurrentChannel (emptyChannel);
           }
        });
    }

    public MyObservable <Boolean> isConnected () {return isConnected;}
    public MyObservable <String> nick () {return nick;}
    public MyObservable <String> server () {return server;}
    public MyObservable <ConvoModel> currentConvo () {return currentConvo;}
    public MyListModel<String> chatLineListModel () {return chatLineListModel;}
    public MyListModel<String> logListModel () {return logListModel;}
    public DefaultListModel<ConvoModel> channelListModel () {return channelListModel;}
    public DefaultListModel<ConvoModel> userListModel () {return userListModel;}    
    public String myNick () {return nick.get();}
    
    /**
     * Install a list of channels into our model.
     * 
     * <p> Notice we still don't have current channel nor convo.
     * 
     * @param channelv List of channels as string array of channel names
     */
    public void setChannels (String [] channelv) {        
        channelListModel.clear();
        userListModel.clear();
        for (String channelName: channelv) {
            ChannelModel channel = getOrCreateChannel (channelName);
            channelListModel.addElement(channel.getChannelConvo());
        }        
    }
    
    /**
     * Set a channel as current.
     * 
     * <p>If channel name is not current, get it from our storage. 
     * If we don't have model for it, create it.
     * <p>Once we have the channel model, set the usersmodel and currentChannel
     * CurrentConvo will be the channel convo.
     * 
     * @param channelName channel to set as current
     */
    public void setCurrentChannel (String channelName) {
        ChannelModel channel = getOrCreateChannel (channelName);
        setCurrentChannel (channel);
    }
    
    private void setCurrentChannel (ChannelModel newChannel) {
        if (newChannel.getChannelConvo().sameAs (currentConvo.get())) return;
        
        if (!newChannel.sameAs(currentChannel)) {
            newChannel.setAsCurrent();
        }
        currentChannel.getChannelConvo().setAsCurrent();
    }

    /**
     * Set as current convo, a private convo to the specified user.
     * 
     * <p>If first time we chat with this user, create a convo model for it.
     * 
     * @param chatMate user to chat with
     */    
    public void setCurrentChatmate (String chatMate) {       
        ConvoModel convo = getOrCreatePrivateConvo (chatMate);
        if (convo.sameAs (currentConvo.get())) return;
        convo.setAsCurrent();
    }

    /**
     * Get a channel model for a given channel name, or create new if we don't have it.
     * 
     * @param channelName channel name for which we want the channel model
     * @return the channel model.
     */    
    public ChannelModel getOrCreateChannel (String channelName) {
        ChannelModel cm = channels.get (channelName);
        if (cm == null) {
            cm = new ChannelModel (channelName);
            channels.put(channelName, cm);
        }
        return cm;
    }    

    /**
     * Get a convo model for a given user, or create new if we don't have it.
     * 
     * @param chatMate user name fir which we want the convo model
     * @return the user convomodel
     */
    public ConvoModel getOrCreatePrivateConvo (String chatMate) {
        ConvoModel cc = privateConvos.get (chatMate);
        if (cc == null) {
            cc = new ConvoModel (chatMate, null);
            privateConvos.put (chatMate, cc);
        }
        return cc;
    }

    public boolean hasChannel(String chann) {
        for (int i=0; i<channelListModel.getSize(); i++) {
            if (channelListModel.getElementAt(i).getConvoName().equals(chann)) return true;
        }
        return false;
    }

    /**
     * Channel UI model
     * 
     * <p> keeps all data needed to represent a channel
     * <p> it keeps the channel name, the list of users and the channel conversation.
     * 
     */
    public class ChannelModel {
        private final String channelName;
        private final Set<String> users;
        private final ConvoModel convo;
        private boolean autoJoin;
        
        public ChannelModel (String channelName) {
            this.channelName = channelName;
            this.convo = new ConvoModel (channelName, this);
            this.users = new LinkedHashSet <>(); // retain order of additions.
            this.autoJoin = true;
        }
        
        public boolean isEmpty () {return channelName.equals("");}
        public String getName () {return channelName;}
        public ConvoModel getChannelConvo () {return convo;}     
        public boolean hasUsers () {return !users.isEmpty();}
        public boolean isAutoJoin () {return autoJoin;}
        
        private void setAsCurrent () {
            currentChannel = this;
            userListModel.clear();
            for (String user: users) userListModel.addElement (getOrCreatePrivateConvo(user));            
        }
            
        public void addUser (String user) {
            if (users.contains(user)) return;
            users.add(user);
            if (isCurrentChannel ()) {
                userListModel.addElement (getOrCreatePrivateConvo(user));
            }
        }

        public void removeUser (String user) {
            users.remove(user);
            if (isCurrentChannel ()) {
                userListModel.removeElement (getOrCreatePrivateConvo(user));
            }
        }
        
        public void doModelJoin (String [] userv) {
            users.clear();
            for (String user: userv) users.add(user);
            if (isCurrentChannel ()) {
                userListModel.clear();
                for (String user: users) userListModel.addElement (getOrCreatePrivateConvo(user));
            }
            currentConvo.set(this.getChannelConvo());
        }

        public void doModelLeave () {
            doModelJoin (new String [] {});
            this.autoJoin = false;
        }

        public boolean sameAs (ChannelModel other) {
            if (other == null) return false;
            return channelName.equals(other.channelName);
        }
        public boolean isCurrentChannel () {return this.sameAs(currentChannel);}

    }
       
    
    /**
     * Chat conversation UI model
     * 
     * <p>Keeps data as needed to represent one chat conversarion.
     * <p>It keeps: destination name (channel or chatMate), isPrivate: whether destination is user or not and
     * the list of chat lines
     */    
    public class ConvoModel {
        
        private final String convoName;
        private final boolean isPrivate;        
        private final List<ChatLine> chatLines;
        private final DefaultListModel<ConvoModel> listModel;
        private final ChannelModel parent;
        private boolean hasUnreadMessages;
                
        public ConvoModel (String destination, ChannelModel channel) { 
            this.parent = channel; // only for channel convos
            this.chatLines = new LinkedList<>();
            this.isPrivate = channel == null;
            this.convoName = destination;
            this.listModel = isPrivate ? userListModel : channelListModel;
        }
        
        public String getConvoName () {return convoName;}
        public boolean isPrivate () {return isPrivate;}
        public boolean hasUnreadMessages () {return hasUnreadMessages;}
        
        private void setAsCurrent () {
            hasUnreadMessages = false;
            currentConvo.set (this);
            chatLineListModel.clear();
            for (ChatLine chatLine: chatLines) chatLineListModel.addElement(chatLine.getText());                            
        }
        
        public void addLine (String line) {
            chatLines.add (new ChatLine (line));
            if (isCurrentConvo()) {
                // Update swing JList
                chatLineListModel.addElement(line); 
            } else {
                // if this is not the current convo, set the "has unread messages"
                // and update the "list of channels/users" , so that it is redisplayed.
                hasUnreadMessages = true;
                int idx = listModel.indexOf(this);
                if (idx >=0) {
                    listModel.set(idx, this);
                }
            }
        }

        //public List<String> getLines () {
        //    return chatLines.stream().map( line -> line.toString()).collect(Collectors.toList());
        //}
       
        // Get channel. Only valid for Channel convos, not private convos.
        public ChannelModel getChannel () {
            return parent;
        }
               
        public int numLines () {
            return chatLines.size();
        }
        
        public boolean sameAs (ConvoModel other) {
            if (other == null) return false;
            if (other.isPrivate != isPrivate) return false;
            return convoName.equals(other.convoName);
        }
        
        public boolean isCurrentConvo () {
            return this.sameAs (currentConvo.get());
        }


        /**
         * String to show as item inside a JList. It has basic html to enhance visual style.
         * 
         * @return the string to show
         */
        public String toHTML () {
            String html;
            String sym = "&#9733;"; //"&#8227;";
            String sym_b = "&#8199";//"&#8194"; // unicode space.
            
            if (hasUnreadMessages) html = "<html><span style=\"color:red;\">"+sym+"</span>"+convoName+"</html>";
            else html = "<html>"+convoName+"</html>";

            return html;
        }

        public String toString () {
            return toHTML();
        }
        
    }
    
    
    /**
     * Single chat line UI model.
     * 
     * <p> keeps data to represent one line of chat.
     * 
     */    
    public static class ChatLine {
        private String line;
        boolean delivered;
        boolean seen;
        
        public ChatLine (String line) {
            this.line = line; 
            this.delivered = this.seen = false;
        }
        
        public String getText () {return line;}        
    }    
    
        
}
