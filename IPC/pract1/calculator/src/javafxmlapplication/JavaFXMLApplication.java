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

class updateTextThread extends Thread {
    private static updateTextThread prevThread = null;
    private static TextField textField;
    
    private final int CHAR_INTERVAL = 40;
    private final int CHAR_DELETE_INTERVAL = 10;
    private final int TEXT_INTERVAL = 1500;
    private final int TEXT_DELETE_INTERVAL = 200;
    private String[] texts;
    
    public static void setTextField(TextField tf) {
        textField = tf;
    }
    
    public updateTextThread(String[] texts) {
        this.texts = texts;
    }
    
    private static void killPrevThread() {
        // Deprecated methods are fun!
        if (prevThread != null) prevThread.stop();
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
        
        for (String t : texts) {
            for (int i = 0; i <= t.length(); i++) {
                final int limit = i;
                Platform.runLater(() -> textField.setText(t.substring(0, limit)));
                delay(CHAR_INTERVAL);
            }
            
            delay(TEXT_INTERVAL);
            
            for (int i = t.length(); i >= 0; i--) {
                final int limit = i;
                Platform.runLater(() -> textField.setText(t.substring(0, limit)));
                delay(CHAR_DELETE_INTERVAL);
            }
            
            delay(TEXT_DELETE_INTERVAL);
        }
    }
}