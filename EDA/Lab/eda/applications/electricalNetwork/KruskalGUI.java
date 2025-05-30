package applications.electricalNetwork;

import java.awt.Panel;
import java.awt.TextField;
import java.awt.Button;
import java.awt.Font;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Insets;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.BasicStroke;

import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;

import java.awt.event.ActionListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseListener;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;

import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;

import java.io.File;
import java.io.IOException;

import java.util.ArrayList;
import libraries.dataStructures.models.Map;
import libraries.dataStructures.models.ListPOI;
import libraries.dataStructures.linear.LinkedListPOI;
import libraries.dataStructures.scattered.HashTable;
import libraries.dataStructures.graphs.Adjacent;

/** Application to test, with a GUI, a MunicipalitiesNetGraph
 *
 * @version (Curso 2022/23)
 */

public class KruskalGUI extends JFrame
    implements ActionListener, MouseMotionListener, MouseListener  {

    // Name of the image file that contains the map
    private static final String MAP_FILE =   "applications" + File.separator
                                           + "electricalNetwork" + File.separator
                                           + "spain.jpg";

    private static final String NO_MAP_MSG =   "Could not load the image file with the map";
    private static final String INTRO_MSG = "";

    // Municipalities graph
    private MunicipalityNetGraph gg;

    // Main municipalities
    private ArrayList<Municipality> municipalities;

    private boolean problemLoaded = false;

    // Elements of the graphical interface
    // Panels
    private Panel municipalitiesPanel;
    // Campos de texto
    private TextField fileTextField;
    private JTextArea messagesTextArea;
    // Botones
    private Button loadProblemButton, computeButton;
    // Componente para la gestion del mapa
    private MapComponent map;
    // Fuentes para el texto
    private Font titlesFont, normalFont;
    // Coordenadas del raton sobre el mapa
    private int mouseX, mouseY;

    /** Inicializa el gestor de municipios y la interfaz grafica */
    public KruskalGUI() {
        super("Intercity Electrical Network");

        // Configuracion de la ventana
        setLayout(null);
        setBackground(Color.lightGray);
        setSize(1010, 618);
        setResizable(false);
        setLocationRelativeTo(null);

        // Fuentes
        titlesFont = new Font("ARIAL", Font.BOLD, 12);
        normalFont = new Font("ARIAL", Font.PLAIN, 11);

        // Paneles, definicion de tamanyos, posiciones y caracteristicas
        municipalitiesPanel = new Panel();
        municipalitiesPanel.setBounds(4, 4, 192, 582);
        municipalitiesPanel.setLayout(null);
        municipalitiesPanel.setBackground(Color.lightGray);
        add(municipalitiesPanel);
        inicializarPanelMunicipios();

        // Cargar la imagen del mapa
        try {
            BufferedImage mapImage = ImageIO.read(new File(MAP_FILE));
            map = new MapComponent(mapImage);
            map.setBounds(200, 4, 999, 603);
            map.addMouseMotionListener(this);
            map.addMouseListener(this);
            add(map);
        } catch (IOException e) {
            JOptionPane.showMessageDialog(this, NO_MAP_MSG, "Error",
                JOptionPane.ERROR_MESSAGE);
            System.exit(0);
        }
        setVisible(true);
    }

    //Inicializa el panel donde se muestra la informacion sobre el camino minimo
    private void inicializarPanelMunicipios() {
        // Etiquetas y campos de texto
        createLabel("Electrical Network to be optimized", 4, 20, 160, 16,
                municipalitiesPanel, true);
        createLabel("Problem:", 4, 60, 60, 40, municipalitiesPanel, true);
        fileTextField = createTextField(60, 60, 80, 40, municipalitiesPanel);
        createLabel("Result:", 4, 260, 120, 16, municipalitiesPanel, true);

        loadProblemButton = createButton("Load Problem", 60, 150, 120, 40,
                municipalitiesPanel);
        // Botones
        computeButton = createButton("Compute Optimal", 60, 200, 120, 40,
                municipalitiesPanel);
        computeButton.disable();
        // Panel de resultados
        messagesTextArea = new JTextArea(INTRO_MSG);
        messagesTextArea.setFont(normalFont);
        messagesTextArea.setEditable(false);
        // taMsgs.setLineWrap(true);
        // taMsgs.setWrapStyleWord(true);
        JScrollPane scrollPane = new JScrollPane(messagesTextArea);
        scrollPane.setBounds(4, 300, 185, 50); //(4, 155, 185, 300);
        municipalitiesPanel.add(scrollPane);
    }

    // Crea una etiqueta de texto
    // text    Texto a mostrar en la etiqueta
    // @param   x       Coordenada horizontal de la etiqueta
    // @param   y       Coordenada vertical de la etiqueta
    // @param   width   Anchura en pixels de la etiqueta
    // @param   height  Altura en pixels de la etiqueta
    // @param   p       Panel que contendra la etiqueta
    // @param   title   True si es una etiqueta de titulo (fuente mas grande)
    private JLabel createLabel(String text, int x, int y, int width,
                               int height, Panel p, boolean title) {
        JLabel label = new JLabel(text);
        if (title) { label.setFont(titlesFont); }
        else { label.setFont(normalFont); }
        label.setBounds(x, y, width, height);
        p.add(label);
        return label;
    }

    //Crea un campo de texto
    // @param   x       Coordenada horizontal del campo de texto
    // @param   y       Coordenada vertical del campo de texto
    // @param   width   Anchura en pixels del campo de texto
    // @param   height  Altura en pixels del campo de texto
    // @param   p       Panel que contendra el campo de texto
    private TextField createTextField(int x, int y, int width,
                                      int height, Panel p) {
        TextField text = new TextField();
        text.setBounds(x, y, width, height);
        p.add(text);
        return text;
    }

    // Crea un boton
    // @param   texto   Texto a mostrar en el boton
    // @param   x       Coordenada horizontal del boton
    // @param   y       Coordenada vertical del boton
    // @param   width   Anchura en pixels del boton
    // @param   height  Altura en pixels del boton
    // @param   p       Panel que contendra el boton
    private Button createButton(String texto, int x, int y, int width,
                                int height, Panel p) {
        Button b = new Button(texto);
        b.setBounds(x, y, width, height);
        b.setBackground(Color.LIGHT_GRAY);
        b.addActionListener(this);
        p.add(b);
        return b;
    }

    // Muestra un mensaje en el area de resultados de las acciones realizadas
    // @param   msg     Mensaje a mostrar
    private void message(String msg) {
        messagesTextArea.append("\n" + msg);
        messagesTextArea.setCaretPosition(messagesTextArea.getDocument().getLength());
    }

    /** Evento que ocurre al pulsar un boton: se invoca al metodo asociado
     *  al boton pulsado
     * @param   a   Informacion relativa al evento
     */
    public void actionPerformed(ActionEvent a) {

        if (a.getSource() == loadProblemButton) {

            try {
                gg = new MunicipalityNetGraph(fileTextField.getText());
                problemLoaded = true;
                String text = "Problem loaded correctly.";
                text += "\n Number of municipalities = " + gg.numVertices();
                text += "\n Number of electrical lines = "
                        + gg.numEdges();
                messagesTextArea.setText(text);
                computeButton.enable();
                municipalities = new ArrayList<Municipality>();
                for (int i = 0; i < gg.numVertices(); i++) {
                    municipalities.add(gg.getMunicipality(i));
                }
                map.dibujarGrafo(gg);

            } catch (Exception eChecked) {
                //Ha ocurrido algun problema con el fichero
                System.err.println(eChecked);
                computeButton.disable();
                gg = null; map.dibujarGrafo(null);
            }
        }
        if (a.getSource() == computeButton) {
            if (problemLoaded) {
                messagesTextArea.setText(arbolExpansionMinima());
            } else {
                messagesTextArea.setText("First you must load a problem (P1 or P2)");
            }
        }
    }

    private String arbolExpansionMinima() {
        double coste = gg.createKruskalAdjacents();
        Map<Municipality, ListPOI<Municipality>> mST = gg.kruskalAdjacents;
        String texto = "Unable to obtain the Minimum Spanning Tree";
        if (mST != null) {
            map.dibujarArbol(mST);
            texto = String.format("Total cost = %6.2f mil. \u20ac\n",
                                  coste);
        }
        return texto;
    }

    /**Evento que ocurre al hacer click con el raton
     * @param   e   Informacion relativa al evento
     */
    public void mouseClicked(MouseEvent e) {   }

    /**Evento que ocurre cuando el cursor del raton entra en el marco
     * de la aplicacion
     * @param   e   Informacion relativa al evento
     */
    public void mouseEntered(MouseEvent e) {   }

    /** Evento que ocurre cuando el cursor del raton sale del marco
     * de la aplicacion
     * @param   e   Informacion relativa al evento
     */
    public void mouseExited(MouseEvent e) {    }

    /**Evento que ocurre al pulsar un boton del raton
     * Boton izquierdo: hacemos zoom sobre el mapa si el raton esta sobre los
     *                  botones de zoom
     * Boton derecho: mostramos la informacion del municipio sobre el que se
     *                encuentra el raton
     * @param   e   Informacion relativa al evento
     */
    public void mousePressed(MouseEvent e) {
        if (e.getButton() == MouseEvent.BUTTON1) {
            mouseX = e.getX();
            mouseY = e.getY();
            if (mouseX > 8 && mouseX < 28 && mouseY > 553 && mouseY < 573) {
                map.zoomOut();
            }
            else if (mouseX > 32 && mouseX < 52
                     && mouseY > 553 && mouseY < 573) {
                map.zoomIn();
            }
        } else if (e.getButton() == MouseEvent.BUTTON3) {
            Municipality m = null;
            int x = e.getX(), y = e.getY();
            for (int i = 1; i <= gg.numVertices() && m == null; i++) {
                Municipality aux = gg.getMunicipality(i);
                if (map.municipioSeleccionado(aux, x, y)) { m = aux; }
            }
            if (m != null) {
                JOptionPane.showMessageDialog(this,
                        String.format("%s\nPop.: %d people\nArea: %s km2",
                                m.getName(), m.getPopulation(), m.getArea()),
                        "Message", JOptionPane.INFORMATION_MESSAGE);
            }
        }
    }

    /**Evento que ocurre al soltar un boton del raton
     * @param   e   Informacion relativa al evento
     */
    public void mouseReleased(MouseEvent e) {   }

    /** Evento que ocurre al mover el raton
     * @param   e   Informacion relativa al evento
     */
    public void mouseMoved(MouseEvent e) {     }

    /** Evento que ocurre al mover el raton manteniendo un boton
     * del raton pulsado
     * Nos desplazamos por el mapa
     * @param   e   Informacion relativa al evento
     */
    public void mouseDragged(MouseEvent e) {
        int incX = mouseX - e.getX();
        int incY = mouseY - e.getY();
        mouseX = e.getX();
        mouseY = e.getY();
        map.moveZoom(incX, incY);
    }

    /**Metodo principal: crea la interfaz grafica de la aplicacion
     * @param  args   Argumentos de la linea de comandos (no se utiliza)
     */
    public static void main(String[] args) {
        new KruskalGUI();
    }

    /** Componente para la gestion del mapa */
    class MapComponent extends JComponent {

        // Arbol a mostrar, otra representacion del grafo con tabla hash
        Map<Municipality, ListPOI<Municipality>> municDict;
        // Imagen que contiene el mapa
        private BufferedImage map;
        // Muestra todas las aristas del grafo si esta a true
        private boolean drawAll;
        // Nivel de zoom
        private double zoom;
        // Coordenadas origen actuales del mapa
        private int zoomX, zoomY;

        /**Constructor: inicializa el mapa
         * @param  m   Imagen que contiene el mapa
         */
        MapComponent(BufferedImage m) {
            this.map = m;
            setCursor(Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR));
            drawAll = true;
            zoomX = 0; zoomY = 0;
            zoom = 1.0;

        }

        /**Devuelve las dimensiones del mapa
         * @return      Dimensiones preferidas del componente
         */
        public Dimension getPreferredSize() {
            Insets insets = getInsets();
            int w = insets.left + insets.right + map.getWidth();
            int h = insets.top + insets.bottom + map.getHeight();
            return new Dimension(w, h);
        }

        /** Incrementa el nivel de zoom del mapa */
        public void zoomOut() {
            if (zoom > 1.0) {
                zoom = zoom / 2.0;
                if (zoom < 1.0) { zoom = 1.0; }
                int w = (int) (map.getWidth() / zoom);
                int h = (int) (map.getHeight() / zoom);
                if (zoomX + w > map.getWidth()) {
                    zoomX = map.getWidth() - w - 1;
                } else if (zoomX < 0) { zoomX = 0; }
                if (zoomY + h > map.getHeight()) {
                    zoomY = map.getHeight() - h - 1;
                } else if (zoomY < 0) { zoomY = 0; }
                repaint();
            }
        }

        /** Reduce el nivel de zoom del mapa */
        public void zoomIn() {
            if (zoom < 5.0) {
                zoom = zoom + 1.0;
                if (zoom > 5.0) { zoom = 5.0; }
                int w = (int) (map.getWidth() / zoom);
                int h = (int) (map.getHeight() / zoom);
                if (zoomX + w > map.getWidth()) {
                    zoomX = map.getWidth() - w - 1;
                } else if (zoomX < 0) { zoomX = 0; }
                if (zoomY + h > map.getHeight()) {
                    zoomY = map.getHeight() - h - 1;
                } else if (zoomY < 0) { zoomY = 0; }
                repaint();
            }
        }

        /** Desplaza el mapa */
        public void moveZoom(int incX, int incY) {
            if (zoom != 1.0) {
                zoomX += incX;
                zoomY += incY;
                int w = (int) (map.getWidth() / zoom);
                int h = (int) (map.getHeight() / zoom);
                if (zoomX + w > map.getWidth()) {
                    zoomX = map.getWidth() - w - 1;
                } else if (zoomX < 0) { zoomX = 0; }
                if (zoomY + h > map.getHeight()) {
                    zoomY = map.getHeight() - h - 1;
                } else if (zoomY < 0) { zoomY = 0; }
                repaint();
            }
        }

        public void dibujarGrafo(MunicipalityNetGraph gg) {
            if (gg == null) {
                zoomX = 0; zoomY = 0;
                zoom = 1.0;
                repaint();
            }
            else {
                int numV = gg.numVertices();
                Map<Municipality, ListPOI<Municipality>> munic =
                    new HashTable<Municipality, ListPOI<Municipality>>(numV);
                for (int i = 0; i < numV; i++) {
                    Municipality m1 = gg.getMunicipality(i);
                    ListPOI<Adjacent> ady = gg.adjacentTo(i);
                    ListPOI<Municipality> mAdy = new LinkedListPOI<Municipality>();
                    for (ady.begin(); !ady.isEnd(); ady.next()) {
                        int j = ady.get().getTarget();
                        Municipality m2 = gg.getMunicipality(j);
                        mAdy.add(m2);
                    }
                    munic.put(m1, mAdy);
                }

                drawAll = true;

                int minX, minY, maxX, maxY, w, h;
                ListPOI<Municipality> c = munic.keys();
                c.begin();
                Municipality m0 = c.get();
                minX = m0.getPosX(); maxX = minX;
                minY = m0.getPosY(); maxY = minY;
                for (; !c.isEnd(); c.next()) {
                    Municipality mi = c.get();
                    int px = mi.getPosX(), py = mi.getPosY();
                    if (px < minX) { minX = px; }
                    else if (px > maxX) { maxX = px; }
                    if (py < minY) { minY = py; }
                    else if (py > maxY) { maxY = py; }
                }
                double ar = map.getWidth() / (double) map.getHeight();
                if (maxX - minX < ar * (maxY - minY)) {
                    h = maxY - minY + 20;
                    w = (int) (h * ar);
                } else {
                    w = maxX - minX + 30;
                    h = (int) (w / ar);
                }
                zoom = map.getWidth() / w;
                if (zoom <= 1.2) {
                    zoom = 1.0;
                    zoomX = 0; zoomY = 0;
                }
                else {
                    if (zoom > 5.0) {
                        w = (int) ((w / 5.0) * zoom);
                        h = (int) ((h / 5.0) * zoom);
                        zoom = 5.0;
                    }
                    zoomY = (int) ((maxY + minY) / 2.0 - h / 2.0);
                    zoomX = (int) ((maxX + minX) / 2.0 - w / 2.0);
                    if (zoomX < 0) { zoomX = 0; }
                    if (zoomY < 0) { zoomY = 0; }
                }

                repaint();
            }
        }

        public void dibujarArbol(Map<Municipality,
                                 ListPOI<Municipality>> municDict) {
            this.municDict = municDict;
            if (municDict.size() == 0) {
                //zoomX = 0; zoomY = 0;
                //zoom = 1.0;
                drawAll = true;
            } else {
                drawAll = false;
            }

            int minX, minY, maxX, maxY, w, h;
            ListPOI<Municipality> c = municDict.keys();
            c.begin();
            Municipality m0 = c.get();
            minX = m0.getPosX(); maxX = minX;
            minY = m0.getPosY(); maxY = minY;
            for (; !c.isEnd(); c.next()) {
                Municipality mi = c.get();
                int px = mi.getPosX(), py = mi.getPosY();
                if (px < minX) { minX = px; }
                else if (px > maxX) { maxX = px; }
                if (py < minY) { minY = py; }
                else if (py > maxY) { maxY = py; }
            }
            double ar = map.getWidth() / (double) map.getHeight();
            if (maxX - minX < ar * (maxY - minY)) {
                h = maxY - minY + 20;
                w = (int) (h * ar);
            } else {
                w = maxX - minX + 30;
                h = (int) (w / ar);
            }
            zoom = map.getWidth() / w;
            if (zoom <= 1.2) {
                zoom = 1.0;
                zoomX = 0; zoomY = 0;
            } else {
                if (zoom > 5.0) {
                    w = (int) ((w / 5.0) * zoom);
                    h = (int) ((h / 5.0) * zoom);
                    zoom = 5.0;
                }
                zoomY = (int) ((maxY + minY) / 2.0 - h / 2.0);
                zoomX = (int) ((maxX + minX) / 2.0 - w / 2.0);
                if (zoomX < 0) { zoomX = 0; }
                if (zoomY < 0) { zoomY = 0; }
            }
            repaint();
        }

        // Transforma la coordenada horizontal del mapa en coordenada
        // horizontal de la pantalla
        // @param  x   Coordenada horizontal del mapa
        // @return     Coordenada horizontal de la pantalla
        private int posX(int x) { return (int) ((x - zoomX) * zoom); }

        // Transforma la coordenada vertical del mapa en coordenada
        // vertical de la pantalla
        // @param  x   Coordenada vertical del mapa
        // @return     Coordenada vertical de la pantalla
        private int posY(int y) { return (int) ((y - zoomY) * zoom); }

        // Dibuja los botones de zoom sobre el mapa
        // @param  g   Superficie de dibujo
        private void paintZoomButtons(Graphics g) {
            g.setColor(Color.blue);
            g.fillRoundRect(8, 553, 20, 20, 6, 6);
            g.fillRoundRect(32, 553, 20, 20, 6, 6);
            g.setColor(Color.white);
            g.drawString("-", 17, 567);
            g.drawString("+", 38, 567);
        }

        /** Comprueba si un municipio esta en las coordenadas especificadas
         * @param  m    Municipio a comprobar
         * @param  mX   Coordenada horizontal
         * @param  mY   Coordenada vertical
         * @return True si el municipio esta en dichas coordenadas
         */
        public boolean municipioSeleccionado(Municipality m, int mX, int mY) {
            boolean sel = false;
            int posX = posX(m.getPosX());
            if (Math.abs(posX - mX) < 6) {
                int posY = posY(m.getPosY());
                sel = Math.abs(posY - mY) < 6;
            }
            return sel;
        }

        // Dibuja un municipio sobre el mapa
        // @param   g   Superficie de dibujo
        // @param   m   Municipio a mostrar
        private void dibujarMunicipio(Graphics g, Municipality m) {
            int posX = posX(m.getPosX());
            int posY = posY(m.getPosY());
            g.setColor(Color.BLACK);
            g.drawOval(posX - 7, posY - 7, 13, 13);
            g.setColor(Color.RED);
            g.fillOval(posX - 4, posY - 4, 8, 8);
            g.setColor(Color.BLACK);
            g.drawString(m.getName(), posX + 10, posY + 10);
        }

        protected void drawEdge(Graphics g, Municipality m1, Municipality m2) {
            g.drawLine(posX(m1.getPosX()),
                       posY(m1.getPosY()),
                       posX(m2.getPosX()),
                       posY(m2.getPosY()));
            dibujarMunicipio(g, m1);
            dibujarMunicipio(g, m2);
        }

        /**Procedimiento de dibujado del mapa
         * @param   g   Superficie de dibujo
         */
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            Graphics2D g2d = (Graphics2D) g;
            g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                RenderingHints.VALUE_ANTIALIAS_ON);
            g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
                RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
            int w = (int) (map.getWidth() / zoom);
            int h = (int) (map.getHeight() / zoom);
            g.drawImage(map, 0, 0, map.getWidth() - 1, map.getHeight() - 1,
                zoomX, zoomY, zoomX + w, zoomY + h, null);
            float red;

            if (gg != null) {
                if (drawAll) {
                    for (int i = 0; i < gg.numVertices(); i++) {
                        Municipality m1 = gg.getMunicipality(i);
                        ListPOI<Adjacent> ady = gg.adjacentTo(i);
                        for (ady.begin(); !ady.isEnd(); ady.next()) {
                            int j = ady.get().getTarget();
                            Municipality m2 = gg.getMunicipality(j);
                            int totalP = m1.getPopulation() + m2.getPopulation();
                            red = (totalP) / 10000.0f;
                            if (red > 1.0f) { red = 1.0f; }
                            red /= 4.0f;
                            g.setColor(new Color(0.6f - red, 0.5f - red, 0.6f));
                            g.drawLine(posX(m1.getPosX()),
                                       posY(m1.getPosY()),
                                       posX(m2.getPosX()),
                                       posY(m2.getPosY()));
                        }
                    }
                    for (int i = 0; i < municipalities.size(); i++) {
                        //System.out.println(municipiosPrincipales.get(i));
                        dibujarMunicipio(g, municipalities.get(i));
                    }
                }
                else {
                    for (int i = 0; i < gg.numVertices(); i++) {
                        Municipality m1 = gg.getMunicipality(i);
                        ListPOI<Adjacent> ady = gg.adjacentTo(i);
                        for (ady.begin(); !ady.isEnd(); ady.next()) {
                            int j = ady.get().getTarget();
                            Municipality m2 = gg.getMunicipality(j);
                            int totalP = m1.getPopulation() + m2.getPopulation();
                            red = (totalP) / 10000.0f;
                            if (red > 1.0f) { red = 1.0f; }
                            red /= 4.0f;
                            g.setColor(new Color(0.6f - red, 0.5f - red, 0.6f));
                            g.drawLine(posX(m1.getPosX()),
                                       posY(m1.getPosY()),
                                       posX(m2.getPosX()),
                                       posY(m2.getPosY()));
                        }
                    }
                    for (int i = 0; i < municipalities.size(); i++) {
                        //System.out.println(municipiosPrincipales.get(i));
                        dibujarMunicipio(g, municipalities.get(i));
                    }

                    ListPOI<Municipality> lM = this.municDict.keys();
                    for (lM.begin(); !lM.isEnd(); lM.next()) {
                        Municipality ori = lM.get();
                        dibujarMunicipio(g, ori);
                        ListPOI<Municipality> lMAdy =
                            this.municDict.get(lM.get());
                        lMAdy.begin();
                        for (; !lMAdy.isEnd(); lMAdy.next()) {
                            Municipality ady = lMAdy.get();
                            dibujarMunicipio(g, ady);
                            ListPOI<Adjacent> lAdy =
                                gg.adjacentTo(gg.getVertex(ori));
                            double coste = 0.0;
                            lAdy.begin();
                            for (; !lAdy.isEnd(); lAdy.next()) {
                                if (lAdy.get().getTarget()
                                    == gg.getVertex(ady)) {
                                    coste = lAdy.get().getWeight();
                                }
                            }
                            g2d.setStroke(new BasicStroke(3,
                                                     BasicStroke.CAP_ROUND,
                                                     BasicStroke.JOIN_ROUND));
                            String costeSt = String.format("%6.2f", coste);
                            g.drawString(costeSt,
                                    posX((ori.getPosX() + ady.getPosX()) / 2),
                                    posY((ori.getPosY() + ady.getPosY()) / 2));
                            g.drawLine(posX(ori.getPosX()),
                                       posY(ori.getPosY()),
                                       posX(ady.getPosX()),
                                       posY(ady.getPosY()));
                        }
                    }

                }
            }
            paintZoomButtons(g);
        }

    }
}
