/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javafxmlapplication;

import java.awt.Desktop;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;

/**
 *
 * @author jsoler
 */
public class FXMLDocumentController implements Initializable {
    private Label labelMessage;
    @FXML
    private Button BC;
    @FXML
    private Button BDivide;
    @FXML
    private Button BMultiply;
    @FXML
    private Button BSubtract;
    @FXML
    private Button BFunc1;
    @FXML
    private Button BFunc2;
    @FXML
    private Button BFunc3;
    @FXML
    private Button BAdd;
    @FXML
    private Button B7;
    @FXML
    private Button B8;
    @FXML
    private Button B9;
    @FXML
    private Button BEqual;
    @FXML
    private Button B4;
    @FXML
    private Button B5;
    @FXML
    private Button B6;
    @FXML
    private Button BDot;
    @FXML
    private Button B1;
    @FXML
    private Button B2;
    @FXML
    private Button B3;
    @FXML
    private Button B0;
    @FXML
    private TextField TextField;
    
    //=========================================================
    // event handler, fired when button is clicked or 
    //                      when the button has the focus and enter is pressed
    private void handleButtonAction(ActionEvent event) {
        labelMessage.setText("Hello, this is your first JavaFX project - IPC");
    }
    
    //=========================================================
    // you must initialize here all related with the object 
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO
    }    

    @FXML
    private void BCHandler(ActionEvent event) {
    }

    @FXML
    private void BDivideHandler(ActionEvent event) {
    }

    @FXML
    private void BMultiplyHandler(ActionEvent event) {
    }

    @FXML
    private void BSubtractHandler(ActionEvent event) {
    }

    @FXML
    private void BFunc1Handler(ActionEvent event) {
        BFunc2.setText("press");
    }

    @FXML
    private void BFunc2Handler(ActionEvent event) {
        BFunc3.setText("ME");
    }

    @FXML
    private void BFunc3Handler(ActionEvent event) {
        TextField.setText("927883715782816129781278");
    }

    @FXML
    private void BAddHandler(ActionEvent event) {
    }

    @FXML
    private void B7Handler(ActionEvent event) {
    }

    @FXML
    private void B8Handler(ActionEvent event) {
    }

    @FXML
    private void B9Handler(ActionEvent event) {
    }

    @FXML
    private void BEqualHandler(ActionEvent event) {
    }

    @FXML
    private void B4Handler(ActionEvent event) {
    }

    @FXML
    private void B5Handler(ActionEvent event) {
    }

    @FXML
    private void B6Handler(ActionEvent event) {
    }

    @FXML
    private void BDotHandler(ActionEvent event) {
    }

    @FXML
    private void B1Handler(ActionEvent event) {
    }

    @FXML
    private void B2Handler(ActionEvent event) {
    }

    @FXML
    private void B3Handler(ActionEvent event) {
    }

    @FXML
    private void B0Handler(ActionEvent event) {
    }
    
}
