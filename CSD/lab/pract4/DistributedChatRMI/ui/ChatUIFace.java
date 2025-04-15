package ui;

/**
 * Java interface that Graphical User Interfaces must implement.
 * 
 * <p>As of now implementation is done using Java Swing, but from the ChatClient programs
 * perspective, it is not relevant. Whichever user interface that exposes these methods
 * is valid.
 * 
 * <p>We could implement this interface using JavaFX or even some Web interface if
 * we would want it.
 *
 * @author Pablo Galdamez
 */
public interface ChatUIFace {
    
    /**
     * Show error information into the UI
     * 
     * @param msg Message to show
     */
    public void showErrorMessage (String msg);

    /**
     * Show a private message comming from a remote user
     * 
     * @param src Message sender
     * @param dst Message destination. Must be me
     * @param line Received message
     */
    public void showPrivateMessage (String src, String dst, String line);

    /**
     * Show a message delivered to a channel
     * 
     * @param src message sender. It's myself for my own messages
     * @param channel destination channel
     * @param line message to show
     */   
    public void showChannelMessage (String src, String channel, String line);

    /**
     * Show incomming notification of some user leaving a channel
     * 
     * @param channel channel where user is leaving
     * @param nick user who left
     */
    public void showUserLeavesChannel (String channel, String nick);
    
    /**
     * Show incomming notification of some user entering a channel
     * 
     * @param channel channel where user is leaving
     * @param nick user who left
     */
    public void showUserEntersChannel (String channel, String nick);
    
}
