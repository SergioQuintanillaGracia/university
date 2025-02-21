/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javafxmlapplication;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.TextField;
import javafx.stage.Stage;


public class JavaFXMLApplication extends Application {
    
    @Override
    public void start(Stage stage) throws Exception {
        //======================================================================
        // 1- creación del grafo de escena a partir del fichero FXML
        FXMLLoader loader= new  FXMLLoader(getClass().getResource("FXMLDocument.fxml"));
        Parent root = loader.load();
        //======================================================================
        // 2- creación de la escena con el nodo raiz del grafo de escena
        Scene scene = new Scene(root);
        //======================================================================
        // 3- asiganación de la escena al Stage que recibe el metodo 
        //     - configuracion del stage
        //     - se muestra el stage de manera no modal mediante el metodo show()
        stage.setScene(scene);
        stage.setTitle("Calculator");
        stage.setResizable(false);
        stage.show();
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
        
    }
}

class UpdateTextThread extends Thread {
    private static UpdateTextThread prevThread = null;
    private static TextField textField;
    
    // We need to make the variable volatile so changes made to it can instantly
    // be seen by every thread
    volatile boolean running = true;
    
    private String[] texts;
    private static int[] defaultIntervals = new int[] {40, 1500, 10, 200};
    private int[][] intervals;
    
    public UpdateTextThread(String[] texts) {
        int[][] autoIntervals = new int[texts.length][4];
        
        for (int i = 0; i < texts.length; i++) {
            autoIntervals[i] = defaultIntervals;
        }
        
        this.texts = texts;
        this.intervals = autoIntervals;
    }
    
    public UpdateTextThread(String[] texts, int[][] intervals) {
        this.texts = texts;
        this.intervals = intervals;
    }
    
    public static void setTextField(TextField tf) {
        textField = tf;
    }
    
    public static int[] getDefaultIntervals() {
        return defaultIntervals;
    }
    
    private static void killPrevThread() {
        if (prevThread != null) {
            prevThread.running = false;
            prevThread.interrupt();
        }
    }
    
    private void delay(int ms) {
        try {
            sleep(ms);
        } catch (InterruptedException e) {}
    }
    
    @Override
    public void run() {
        killPrevThread();
        prevThread = this;
        
        for (int i = 0; i < texts.length; i++) {
            String t = texts[i];
            
            for (int j = 0; j <= t.length(); j++) {
                if (!running) return;
                final int limit = j;
                Platform.runLater(() -> textField.setText(t.substring(0, limit)));
                delay(intervals[i][0]);
            }
            
            delay(intervals[i][1]);
            
            for (int j = t.length(); j >= 0; j--) {
                if (!running) return;
                final int limit = j;
                Platform.runLater(() -> textField.setText(t.substring(0, limit)));
                delay(intervals[i][2]);
            }
            
            delay(intervals[i][3]);
        }
    }
}