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
import javafx.application.Platform;
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
    public static int buttonPresses = 0;
    
    @FXML
    private Button BC;
    @FXML
    private Button BDivide;
    @FXML
    private Button BMultiply;
    @FXML
    private Button BSubtract;
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
    private TextField textField;
    
    private StoryManager storyManager;
    
    //=========================================================
    // you must initialize here all related with the object 
        @Override
    public void initialize(URL url, ResourceBundle rb) {
        UpdateTextThread.setTextField(textField);
        storyManager = new StoryManager(BC, BDivide, BMultiply, BSubtract,
                BAdd, B7, B8, B9, BEqual, B4, B5, B6, BDot, B1, B2, B3, B0,
                textField);
    }    

    @FXML
    private void BCHandler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleBC();
    }

    @FXML
    private void BDivideHandler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleBDivide();
    }

    @FXML
    private void BMultiplyHandler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleBMultiply();
    }

    @FXML
    private void BSubtractHandler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleBSubtract();
    }

    @FXML
    private void BAddHandler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleBAdd();
    }

    @FXML
    private void B7Handler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleNumber();
    }

    @FXML
    private void B8Handler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleNumber();
    }

    @FXML
    private void B9Handler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleNumber();
    }

    @FXML
    private void BEqualHandler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleBEqual();
    }

    @FXML
    private void B4Handler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleNumber();
    }

    @FXML
    private void B5Handler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleNumber();
    }

    @FXML
    private void B6Handler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleNumber();
    }

    @FXML
    private void BDotHandler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleDot();
    }

    @FXML
    private void B1Handler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleNumber();
    }

    @FXML
    private void B2Handler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleNumber();
    }

    @FXML
    private void B3Handler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleNumber();
    }

    @FXML
    private void B0Handler(ActionEvent event) {
        buttonPresses++;
        if (!UpdateTextThread.isThreadRunning())
            storyManager.handleNumber();
    }
    
}
