//
// Students need not modify nor fully understand this file.
//

package ui;

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.DefaultListCellRenderer;
import javax.swing.DefaultListSelectionModel;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import javax.swing.border.LineBorder;
import ui.ChatModel.ChannelModel;
import ui.ChatModel.ConvoModel;
import ui.SwingUtils.MyDocumentListener;
import static ui.SwingUtils.setFixedSize;
import static ui.SwingUtils.setMinimumSize;

//
// Students need not modify nor fully understand this file.
//

/**
 * <p>ChatClient graphical user interface. 
 * 
 * <p> It is a simple UI whose purpose is to show 
 * interaction among RMI objects and RMI processes. It's not intended as a flully-fledged UI.
 * Hoewever it is pretty complete and functional.
 * 
 * <p>It is implemented using Swing, so that it is compatible to most Java installations.
 * Tested from Java 8 to Java 15. It OPTIONALLY uses FlatLaf look and feel if it is found in the classpath.
 * Tested version is flatlaf-1.2.jar
 * 
 * <p>This UI implements the "V" in the MVC design pattern. It does not know a word about RMI nor 
 * distribution. "C" is the ChatClient. "M" are the Remote objects.
 * 
 * <p>In terms of UI, there's a model for the UI, called ChatModel. Changes to the model are reflected into the UI
 * Changes to the UI are reflected into the UI model.
 * 
 * <p>ChatClient calls this class public methods (those exposed by interface ChatUIFace)
 * <p>This class calls ChatClient public methods (those exposed by intercace ChatClientFace)
 * 
 * @author Pablo Galdamez
 * 
 */

public class ChatUI implements ChatUIFace
{    
    private String TITLE;
    
    // Real client code that controlls us.
    private final ChatClientFace cc;  // Main client. 
    
    // UI data model. Changes to the UI model will be reflected into swing components.
    // Changes to swing components will be reflected into the UI model (some)
    private final ChatModel model = new ChatModel(); 

    // Optional channel where we autojoin if specified
    private String defaultChannelName;
    
    //
    // Some basic text fields.
    //
    private final JTextField txtMessage = new JTextField (50);  // Message to send
    private final JTextField txtServer = new JTextField (15);   // Server to connect to
    private final JTextField txtNick = new JTextField (10);     // Our own nick
    
    // JButton to trigger connection/disconnection
    private final JButton btnConnect = new JButton ("Disconnect"); 
        
    // JButton to join/leave channel
    private final JButton btnJoin = new JButton ("Leave"); 
        
    // Main area where chat convo appears
    private final JList<String> lstConvo = new JList<> (model.chatLineListModel());     
    
    // Area where error/info messages are shown
    private final JList<String> lstLogs = new JList<> (model.logListModel());       
    
    // Area for channels
    private final JList<ConvoModel> lstChannels = new JList<> (model.channelListModel());   
    
    // Area for users in a channel
    private final JList<ConvoModel> lstUsers = new JList<> (model.userListModel());     
    
    // Current chat destination
    private final JLabel lblConvoDestination = new JLabel ("");

    // Main window
    private final JFrame mainFrame = new JFrame (TITLE);
    
    // Color palette to enhance visual effects
    private final ColorPalette cp = new ColorPalette ();
    
    /**
     * Build a new ChatUI
     * <p>If FlatDarkLaf is available, load it... just to improve the look and feel.
     * 
     * @param cc ChatClient who controlls this yhis ChatUI
     * @param title Title for the chat program main window
     * @param server Server to connect to
     * @param nick Nick to use when connecting
     * @param defaultChannelName Default channel to auto-join
     */
    public ChatUI (ChatClientFace cc, String title, String server, String nick, String defaultChannelName) {       
        this.TITLE = title;
        this.cc = cc;
        this.model.nick().set(nick);
        this.model.server().set(server);
        this.defaultChannelName = defaultChannelName;

        // All swing code must run inside swing single thread.
        //
        SwingUtilities.invokeLater( () -> {
            //
            // add library flatlaf-1.2.jar  --> for nicer looks
            //
            try {  
                UIManager.setLookAndFeel( "com.formdev.flatlaf.FlatDarkLaf" );
                cp.setDarkTheme();
            } catch( Exception ex ) { } // Old fashioned swing style, if not FlatDarkLaf
            
            // create ui components            
            //
            createUI ();
        });
    }
    
    /**
     * Show the UI
     */
    public void show () {

        // All swing code must run inside swing single thread.
        //
        SwingUtilities.invokeLater( () -> {
            mainFrame.setVisible (true);
            txtNick.requestFocusInWindow();
            setupListeners ();            
        });
    }


    /**
     * Setup listeners on the model
     */
    private void setupListeners () {
        
        // On connect/disconnect, change a few colors and other minor details.
        //
        model.isConnected().onChanged(conn -> {
            btnConnect.setText (conn ? "Disconnect" : "Connect");                
            btnConnect.setBackground (conn ? cp.okColor : cp.requiredColor);              
            txtServer.setEnabled (!conn);
            txtNick.setEnabled (!conn);
        });
        
        // Changes on model "current convo" reflected into swing components: label and button
        //
        model.currentConvo().onChanged ( convo -> {            
            if (convo == null || convo.getConvoName().equals("")) {
                lblConvoDestination.setVisible(false);
                btnJoin.setVisible(false);
            } else {
                lblConvoDestination.setText(" " + convo.getConvoName());
                lblConvoDestination.setVisible(true);
                if (convo.isPrivate()) btnJoin.setVisible(false);
                else {                    
                    if (convo.getChannel().hasUsers()) {
                        btnJoin.setText("Leave");
                        btnJoin.setBackground (cp.okColor);
                    } else {
                        btnJoin.setText("Join");
                        btnJoin.setBackground (cp.requiredColor);
                    }
                    btnJoin.setVisible(true);
                }
            }
        });        
    }

    
    //
    // UI is a Swing JPanel with some "simple" components.
    //
    private void createUI () {
                
        JScrollPane scPane;
        Dimension rigidDim = new Dimension (1,25); // help to set height on headers
        
        //
        // Top panel: server, nick and button to connect/disconnect
        //        
        
        Box lPane = Box.createHorizontalBox();
        lPane.add(btnConnect);

        Box rPane = Box.createHorizontalBox();
        rPane.add (new JLabel("Server: "));
        rPane.add (txtServer);
        rPane.add (Box.createHorizontalStrut(20));
        rPane.add (new JLabel("My nick: ")); 
        rPane.add (txtNick);
        
        JPanel topPane = new JPanel(new BorderLayout());
        topPane.add (lPane, BorderLayout.WEST);
        topPane.add (Box.createHorizontalGlue(), BorderLayout.CENTER);
        topPane.add (rPane, BorderLayout.EAST);
        
        txtServer.setText (model.server().get());
        
        //
        // Central panel: channel list, convo and users list.
        //        
        
        // Panel for channel list      
        Box headerBox = Box.createHorizontalBox();
        headerBox.setBorder(new EmptyBorder (2,0,2,0));
        headerBox.add (new JLabel ("Channels") );
        headerBox.add (Box.createRigidArea(rigidDim));
        
        scPane = new JScrollPane();
        scPane.setViewportView (lstChannels);

        JPanel pnlChannels = new JPanel (new BorderLayout());
        pnlChannels.add (headerBox, BorderLayout.NORTH);
        pnlChannels.add (scPane, BorderLayout.CENTER); 

        //        
        // Central area with 2 sub-panels: chats and logs
        //
        
        // header
        
        JPanel pnlMessagesHeader = new JPanel ();
        pnlMessagesHeader.setLayout( new BoxLayout (pnlMessagesHeader, BoxLayout.LINE_AXIS ));
        pnlMessagesHeader.add (new JLabel ("Chatting area"));
        pnlMessagesHeader.add (Box.createHorizontalStrut(3));
        pnlMessagesHeader.add (lblConvoDestination);        
        pnlMessagesHeader.add (Box.createHorizontalGlue());
        pnlMessagesHeader.add (btnJoin);        
        pnlMessagesHeader.add (Box.createRigidArea (rigidDim));
        pnlMessagesHeader.setBorder( new EmptyBorder (2,0,2,0));
        
        //chats
        JScrollPane scChats = new JScrollPane();
        scChats.setViewportView (lstConvo);
        
        //logs
        JScrollPane scLogs = new JScrollPane();
        scLogs.setViewportView (lstLogs);
                
        JPanel twoPanes = new JPanel (new BorderLayout(0,5));
        twoPanes.add (scChats, BorderLayout.CENTER);
        twoPanes.add (scLogs, BorderLayout.SOUTH);
        
        JPanel pnlMessages = new JPanel (new BorderLayout());
        pnlMessages.add (pnlMessagesHeader, BorderLayout.NORTH);      
        pnlMessages.add (twoPanes, BorderLayout.CENTER); 
        
        // Panel for user list
        headerBox = Box.createHorizontalBox();
        headerBox.setBorder( new EmptyBorder (2,0,2,0));
        headerBox.add (new JLabel ("Users") );
        headerBox.add (Box.createRigidArea(rigidDim));
        
        scPane = new JScrollPane();
        scPane.setViewportView (lstUsers);

        JPanel pnlUsers = new JPanel (new BorderLayout());
        pnlUsers.add (headerBox, BorderLayout.NORTH);        
        pnlUsers.add (scPane, BorderLayout.CENTER); 
        
        // Add all 3 parts into the central panel
        JPanel pnlCenter = new JPanel (new BorderLayout(10, 0));
        pnlCenter.add (pnlChannels, BorderLayout.WEST);
        pnlCenter.add (pnlMessages, BorderLayout.CENTER);
        pnlCenter.add (pnlUsers, BorderLayout.EAST);
        
        //
        // Bottom panel: message to send
        //        
        JPanel pnlMessage = new JPanel(new BorderLayout()); 
        pnlMessage.add (new JLabel("Message: "), BorderLayout.WEST); 
        pnlMessage.add (txtMessage, BorderLayout.CENTER); 
        
        JPanel pnlBottom = new JPanel (new BorderLayout(10, 20)); 
        pnlBottom.add (pnlMessage, BorderLayout.CENTER);
        
        //
        // Few colors and visual effects
        //        
        btnConnect.setForeground ( cp.blackishColor );
        btnJoin.setForeground ( cp.blackishColor );                
        lblConvoDestination.setForeground (cp.highlightColor );
        
//        lblConvoDestination.setText ("#Linux");
//        lblConvoDestination.setVisible (true);
                
        //
        // Main frame is a border frame with 3 subpanels.
        //                
        JPanel pnlMain = new JPanel (new BorderLayout(0,10));        
        pnlMain.add (topPane, BorderLayout.NORTH); 
        pnlMain.add (pnlCenter, BorderLayout.CENTER); 
        pnlMain.add (pnlBottom, BorderLayout.SOUTH);
        topPane.setBorder( new EmptyBorder (0,0,10,0));        
        pnlBottom.setBorder( new EmptyBorder (10,0,10,0));
        
        JPanel pnlBorder = new JPanel (new CardLayout (10,10));
        pnlBorder.add (pnlMain, BorderLayout.CENTER);

        mainFrame.setLayout (new BorderLayout());
        mainFrame.add (pnlBorder, BorderLayout.CENTER);
        
        //
        // List stuff
        //        
        lstChannels.setSelectionMode (ListSelectionModel.SINGLE_SELECTION);
        lstUsers.setSelectionMode (ListSelectionModel.SINGLE_SELECTION);
        setReadOnlyList (lstConvo);
        setReadOnlyList (lstLogs);
        lstLogs.setCellRenderer(new LogCellRenderer ()); // Highlight last line
        
        //
        // Some size restrictions
        //        
        setMinimumSize (pnlUsers, new Dimension (120, 0) );
        setMinimumSize (pnlChannels, new Dimension (150, 0) );        
        setMinimumSize (scLogs, new Dimension (0, 80) );
        
        mainFrame.setMinimumSize(new Dimension(570, 300));
        mainFrame.setPreferredSize(new Dimension(900, 500));        
                
        //
        // Focus highlight
        //        
        txtMessage.addFocusListener( new MyFocusListener ());
        txtServer.addFocusListener( new MyFocusListener ());
        txtNick.addFocusListener( new MyFocusListener ());
                
        //
        // Some model listeners
        //
        model.logListModel().onAdition( idx -> {
            lstLogs.ensureIndexIsVisible (idx); // Automatic scroll       
        });
        model.chatLineListModel().onAdition( idx -> {
            lstConvo.ensureIndexIsVisible (idx); // Automatic scroll       
        });

        
        //
        // Listeners: action listeners and some visual tricks
        //                
        mainFrame.addComponentListener(new ComponentAdapter(){
            @Override
            public void componentResized(ComponentEvent e){
                Dimension d = mainFrame.getSize();
                Dimension minD= mainFrame.getMinimumSize();
                if (d.width < minD.width) d.width=minD.width;
                if (d.height < minD.height) d.height=minD.height;
                mainFrame.setSize (d);
            }
        });      
        
        mainFrame.addWindowListener(new WindowAdapter(){
            @Override
            public void windowClosing (WindowEvent e) {onWindowClose();}
        });
        
        btnConnect.addActionListener( actionEvent -> onConnectButtonPressed() );      
        btnJoin.addActionListener( actionEvent -> onJoinButtonPressed() );      
        txtMessage.addActionListener( actionEvent -> onMessageTyped() );      
        txtServer.addActionListener ( actionEvent -> onConnectionParamsChanged() );      
        txtNick.addActionListener ( actionEvent -> onConnectionParamsChanged() );

        txtNick.getDocument().addDocumentListener( new MyDocumentListener (txtNick, model.nick()));
        
        lstChannels.addListSelectionListener ( a -> {   
            if (a.getValueIsAdjusting() || lstChannels.getSelectedIndex()<0) return;
            ConvoModel dst = lstChannels.getSelectedValue();
            onChannelSelected (dst.getConvoName());
        });
        
        lstUsers.addListSelectionListener ( a -> {   
            if (a.getValueIsAdjusting()  || lstUsers.getSelectedIndex()<0) return;
            onUserSelected (lstUsers.getSelectedValue().getConvoName());
        });


        // If we changed L&F we need this
        SwingUtilities.updateComponentTreeUI(mainFrame);        

        // pack and compute all sizes.        
        mainFrame.pack();      
        
        // Disable changing size of some gui parts.
        // btnConnect and btnJoin, to prevent changes when we change the text
        // rPane to prevent visual glitches when disabling items.
        setFixedSize (btnConnect);
        setFixedSize (btnJoin);
        setFixedSize (rPane);
        
        // We start not connected
        btnConnect.setText("Connect");
        btnConnect.setBackground ( cp.requiredColor );
        btnJoin.setVisible(false);

        // same height for all textfields        
        Dimension dim = new Dimension (0, txtServer.getSize().height);
        setMinimumSize (txtMessage, dim);
        
    }

    
    /**
     * Some swing tricks to setup a list as read-only. No focus, no highlights
     * 
     * @param list Jlist to set as read-only
     */
    private void setReadOnlyList (JList list) {
        list.setSelectionMode (ListSelectionModel.SINGLE_SELECTION);
        list.setDragEnabled(false);
        list.setFocusable(false);
        
        list.setSelectionModel( new DefaultListSelectionModel () {
            @Override
            public void setSelectionInterval(int index0, int index1) {
                super.setSelectionInterval(-1, -1);              
            }      
            @Override
            public void addSelectionInterval(int index0, int index1) {
                super.setSelectionInterval(-1, -1);
            }      
        });
        
    }
    
    /**
     * Simple swing focus listener to highlight TextFields when they gain focus
     */
    private class MyFocusListener  implements FocusListener {
        private Border old;
        @Override
        public void focusGained(FocusEvent e) {
            if (!(e.getComponent() instanceof JComponent)) return;
            JComponent c = (JComponent) e.getComponent();
            old = c.getBorder();
            c.setBorder( new LineBorder (cp.colorBlue));
        }
        
        @Override
        public void focusLost(FocusEvent e) {
            if (!(e.getComponent() instanceof JComponent)) return;
            JComponent c = (JComponent) e.getComponent();
            c.setBorder(old);
        } 
    }

    /**
     * This is the way to override list items default painting.   
     * in our case, just sipmle color for errors/info for last line
     */        
    private class LogCellRenderer extends DefaultListCellRenderer {  
        @Override
        public Component getListCellRendererComponent( JList list, Object value, int index, boolean isSelected, boolean cellHasFocus ) {  
            Component c = super.getListCellRendererComponent( list, value, index, isSelected, cellHasFocus ); 
            
            // Just paint special last line.
            if ( list.getModel().getSize()-1 == index) {
                String str = (String)list.getModel().getElementAt(index);
                if (str.startsWith("ERROR"))
                    c.setForeground( cp.colorRed );
                else 
                    c.setForeground( cp.colorBlue );
            }  
            return c;  
        }  
    }  
        
    //
    // Show a "log" message into the log area
    //
    private void doShowLogMessage (String msg) {
        model.logListModel().addElement (msg);
    }

    private void doShowException (Exception e) {
        Throwable cause = e;
        while (cause.getCause() != null) {
            cause = cause.getCause();
        }
        showErrorMessage ( cause + "");
    }
    
    //
    // UI ActionListener
    // User types [Enter] on the Message chat box.... send a message to the appropriate destination
    //
    private void onMessageTyped () {
        String msg = txtMessage.getText();
        String src = model.myNick();
        String dst = model.currentConvo().get().getConvoName();
        txtMessage.setText("");
        
        if (!model.isConnected().get()) {
            showErrorMessage ("Not connected.");
            return;
        }
        
        if ("".equals(dst)) {
            showErrorMessage ("Click on channel or user to select Destination.");
            return;
        }
        
        try {
            if (model.currentConvo().get().isPrivate()) {
                cc.doSendPrivateMessage (dst, msg);
                if (!src.equals(dst)) doShowPrivateMessageSent (dst, msg);
                else doShowLogMessage ("WARN ==> talking to yourself can reframe negative thinking");
            } else {
                cc.doSendChannelMessage (dst, msg); // ChatClient does the real send
            }
        } catch (Exception e) {
            doShowException (e);
        }
        
    }
    
    //
    // UI ActionListener
    // When user clicks on a channel, this code runs on the selected channel
    //    
    private void onChannelSelected (String newChannel) {
        lstUsers.clearSelection (); // now user needs to click again on user to set user as destination
        model.setCurrentChannel (newChannel);
        
        // Get current channel model
        ChannelModel mchan = model.currentConvo().get().getChannel();
        
        // When clicking on a channel, join if not done already
        // If I joined before channel at least has one member, which is me.
        if ( !mchan.hasUsers() && mchan.isAutoJoin() ) {
        
            // Get new channel users      
            String []users;
            try {
                users = cc.doJoinChannel (newChannel);
            } catch (Exception e) {
                doShowException (e);
                return;
            }
  
            mchan.doModelJoin(users); // reflect join in channel
        } else {
            // If I have the channel, just show info
            doShowLogMessage ("OK ==> channel '" + newChannel + "' set as message destination");            
        }
        
        // Grab focus on message text field
        txtMessage.requestFocusInWindow();
    }
    
    //
    // UI ActionListener
    // When user clicks on a user name, this code runs.
    //
    private void onUserSelected ( String chatMate) {
        lstChannels.clearSelection(); // click again to set destination
        model.setCurrentChatmate (chatMate);
        doShowLogMessage ("OK ==> user '" + chatMate + "' set as message destination");
        txtMessage.requestFocusInWindow();
    }
    
    //
    // UI ActionListener
    // If [Enter] pressed on server or nick, try to connect... only if not connected yet.
    //
    private void onConnectionParamsChanged () {
        if (model.isConnected().get()) return;
        connect();
    }
    
    //
    // UI ActionListener
    // Pressing the connect button, means to connect or to disconnect. It acts as a toggle.
    //
    private void onConnectButtonPressed () {
        if (!model.isConnected().get()) connect();
        else disconnect ();
    }
    
    //
    // UI ActionListener
    // Pressing the join button, means to join or to leave current channel.
    //
    private void onJoinButtonPressed () {
        ChannelModel mchan = model.currentConvo().get().getChannel();
        if (mchan.hasUsers()) {
            // leave
            try {
                cc.doLeaveChannel (mchan.getName());
                mchan.doModelLeave(); // reflect in model that we left.
                doShowLogMessage ("OK ==> " + "Left channel " + mchan.getName());
            } catch (Exception e) {
                doShowException (e);
                return;
            }
        } else {
            // join
            try {
                String [] users = cc.doJoinChannel (mchan.getName());
                mchan.doModelJoin (users); // reflect that we joined
            } catch (Exception e) {
                doShowException (e);
                return;
            }
            
        }
    }
    
    //
    // UI ActionListener
    // User closes window, we terminate the program
    //
    private void onWindowClose () {
        cc.doTerminate();
    }

    
    //
    // UI action listener. 
    // This code runs when user clicks on connect
    //
    // Connect to the specified server
    //
    private void connect () {
        String srv = txtServer.getText();
        String nick = model.myNick();
        
        if (srv == null || srv.trim().equals ("")) {
            showErrorMessage ("Server name cannot be empty"); 
            txtServer.requestFocusInWindow();
            return;
        }
        
        if (nick == null || nick.trim().equals ("")) {
            showErrorMessage ("Nick cannot be empty"); 
            txtNick.requestFocusInWindow();
            return;
        }
        
        String [] channels;
        try {
            channels = cc.doConnect (srv, nick); // Real connection now.
        } catch (Exception e) {
            doShowException (e);
            return;
        }
        
        // set channels in UI model      
        model.isConnected().set(true); // set as connected in model
        model.setChannels (channels); // set channel list in model

        // informative log.
        doShowLogMessage ("OK ==> " + "Connected to '" + srv + "' as '" + nick + "'");
        mainFrame.setTitle (TITLE + " [" + nick + "]");
                
        // "click" on channel, if channel was specified
        // 
        String chann = defaultChannelName;
        if (chann != null && model.hasChannel (chann)) {
            onChannelSelected (chann);                  
        }
        
    }
    
    //
    // UI actionListener
    // This code runs when user clicks on Disconnect.
    //
    // Disconnect from server.
    // 
    private void disconnect () {
        // real disconnect.
        cc.doDisconnect (); 
        
        // UI changes to reflect disconnection
        model.isConnected().set (false);        
        
        // just add log message        
        doShowLogMessage ("OK ==> " + "Disconnected");
    }

    //
    // Add one line to the "convo" as a private message I sent
    //
    private void doShowPrivateMessageSent (String dst, String line) {
        doShowMessage (model.getOrCreatePrivateConvo(dst), cp.colorGreen, model.myNick(), line);
    }
    
    //
    // Add one line to the "convo" as a private message I received
    //
    private void doShowPrivateMessageReceived (String src, String line) {
        Color color = src.equals(model.myNick()) ? cp.colorGreen : cp.colorRed;
        doShowMessage (model.getOrCreatePrivateConvo(src), color, src, line);
    }

    //
    // Add a line to the specified convo, with the specified color for the sender
    // Uses html as redering trick for colors.
    //
    private void doShowMessage (ConvoModel convo, Color color, String src, String line) {
        String hColor = ColorPalette.toHex(color);        
        String html = "<html><font color=" + hColor + ">[&nbsp;" + src + "&nbsp;]&nbsp;</font>" + line + "</html>";        
        convo.addLine (html);
    }        
    
        
    //
    // "show" methods. 
    // These methods belong to ChatUIFace as generic interface exposed to chat programs.
    //
    // Threse are the public UI methods called by ChatClient to show details into the UI
    //
    

    /**
     * Show error into the information area
     * 
     * @param msg Message to show
     */
    @Override
    public void showErrorMessage (String msg) {
        SwingUtilities.invokeLater(() -> {
            doShowLogMessage("ERROR: " + msg);
        });
    }

    /**
     * Show a private message comming from a remote user
     * 
     * @param src Message sender
     * @param dst Message destination. Must be me
     * @param line Received message
     */
    @Override
    public void showPrivateMessage (String src, String dst, String line)
    {
        SwingUtilities.invokeLater( () -> {
            if (!model.isConnected().get() || !dst.equals(model.myNick())) {
                showErrorMessage ("Message received while disconnected or connected with different nick");
                return;
            }
            doShowPrivateMessageReceived (src, line);
        });
    }
     
    /**
     * Show a message delivered to a channel
     * 
    * @param src message sender. It's myself for my own messages
    * @param channel destination channel
    * @param line message to show
    */   
    @Override
    public void showChannelMessage (String src, String channel, String line)
    {
        SwingUtilities.invokeLater( () -> {
            Color color = src.equals (model.myNick()) ? cp.colorGreen : cp.colorBlue;
            doShowMessage (model.getOrCreateChannel (channel).getChannelConvo(), color, src, line);
        });
    }
    
    /**
     * Show incomming notification of some user leaving a channel
     * 
     * @param channel channel where user is leaving
     * @param nick user who left
     */
    @Override
    public void showUserLeavesChannel (String channel, String nick) {        
        SwingUtilities.invokeLater( () -> {
            //
            // Remove user from the ChatModel that we keep.
            //
            model.getOrCreateChannel (channel).removeUser(nick);
        });
    }
    
    /**
     * Show incomming notification of some user entering a channel
     * 
     * @param channel channel where user is leaving
     * @param nick user who left
     */
    @Override
    public void showUserEntersChannel (String channel, String nick) {
        SwingUtilities.invokeLater(() -> {
            if (nick.equals (model.myNick())) {                
                //
                // If it's about me, just show a log info
                //
                doShowLogMessage ("OK ==> " + "Entered channel " + channel);
            } else { 
                //
                // create channel into data model if it didn't exist. Then add user.
                //
                model.getOrCreateChannel (channel).addUser (nick);
            }
        });
    }
    

    /**
     * Simple utility class to hold all colors together.
     */    
    private static class ColorPalette {
    
        // Basic colors we use elsewhere. If theme is dark, we change them
        private Color highlightColor = Color.green.darker();
        
        private Color requiredColor = Color.decode("#DDcccc");
        private Color okColor = Color.decode("#ccccdd");
        private Color blackishColor = Color.black.darker();
        private Color colorBlue = Color.blue;
        private Color colorRed = Color.red;
        private Color colorGreen = Color.green.darker();
        

        private static String toHex (Color col) {
            return String.format("#%06x", col.getRGB() & 0xffffff);            
        }
        
        private void setDarkTheme () {
            highlightColor = Color.decode("#B18D55"); //Color.decode("#A8C023");

            colorBlue = Color.decode("#4A88C7");
            colorRed = Color.decode("#F93F3F");
            colorGreen = Color.decode("#6A8759");
        }
    }
        
}
