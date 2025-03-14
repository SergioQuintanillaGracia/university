/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javafxmlapplication;

import java.lang.reflect.Field;
import java.net.URL;
import java.time.LocalDate;
import java.time.temporal.ChronoUnit;
import static java.time.temporal.ChronoUnit.YEARS;
import java.util.List;
import java.util.ResourceBundle;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.value.ChangeListener;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.DateCell;
import javafx.scene.control.DatePicker;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;

public class FXMLRegisterController implements Initializable {

    @FXML
    private Label emailErrorLabel;
    @FXML
    private TextField emailField;
 
    //properties to control valid fields values. 
    private BooleanProperty validEmail;
    private BooleanProperty validPassword;
    private BooleanProperty validRepeatPassword;
    
    // listener to register on textProperty() or valueProperty()
    private ChangeListener<String> listenerEmail;
    private ChangeListener<String> listenerPassword;
    private ChangeListener<String> listenerRepeatPassword;


    @FXML
    private TextField passwordField;
    @FXML
    private Label passwordErrorLabel;
    @FXML
    private TextField repeatPasswordField;
    @FXML
    private Label repeatPasswordErrorLabel;
    @FXML
    private DatePicker datePicker;
    @FXML
    private Label birthdateErrorLabel;
    @FXML
    private Button registerButton;
    @FXML
    private Button cancelButton;

    
    private void checkEmail() {
        String email = emailField.getText();
//        boolean isValid = email.matches("^[A-Za-z0-9+_.-]+@(.+)$");
        boolean isValid = email.matches("^[\\w!#$%&'*+/=?`{|}~^-]+(?:\\.[\\w!#$%&'*+/=?`{|}~^-]+)*@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,6}$");
        validEmail.set(isValid); //actualiza la property asociada
        showError(isValid, emailField, emailErrorLabel); //muestra o esconde el mensaje de error
    }
    
    private void checkPassword() {
        String password = passwordField.getText();
        boolean isValid = password.matches("^(?=.*[0-9])(?=.*[a-zA-Z])\\S{8,15}$");
        validPassword.set(isValid); //actualiza la property asociada
        showError(isValid, passwordField, passwordErrorLabel); //muestra o esconde el mensaje de error
    }
    
    private void checkRepeatPassword() {
        String password = passwordField.getText();
        boolean isValid = password.equals(repeatPasswordField.getText());
        validRepeatPassword.set(isValid); //actualiza la property asociada
        showError(isValid, repeatPasswordField, repeatPasswordErrorLabel); //muestra o esconde el mensaje de error
    }


    private void showError(boolean isValid, Node field, Node errorMessage){
        errorMessage.setVisible(!isValid);
        field.setStyle((isValid ? "" : "-fx-background-color: #FCE5E0"));
    }

    //=========================================================
    // you must initialize here all related with the object 
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        validEmail = new SimpleBooleanProperty(false);
        validPassword = new SimpleBooleanProperty(false);

        //When the field loses focus, the field is validated. 
        emailField.focusedProperty().addListener((obs, oldVal, newVal) -> {
            if (!newVal) {
                checkEmail();
                if (!validEmail.get()) {
                    //If it is not correct, a listener is added to the text or value 
                    //so that the field is validated while it is being edited.
                    if (listenerEmail == null) {
                        listenerEmail = (a, b, c) -> checkEmail();
                        emailField.textProperty().addListener(listenerEmail);
                    }
                }
            }
        });
        
        //When the field loses focus, the field is validated. 
        passwordField.focusedProperty().addListener((obs, oldVal, newVal) -> {
            if (!newVal) {
                checkPassword();
                if (!validPassword.get()) {
                    //If it is not correct, a listener is added to the text or value 
                    //so that the field is validated while it is being edited.
                    if (listenerPassword == null) {
                        listenerPassword = (a, b, c) -> checkEmail();
                        passwordField.textProperty().addListener(listenerPassword);
                    }
                }
            }
        });
        
        //When the field loses focus, the field is validated. 
        repeatPasswordField.focusedProperty().addListener((obs, oldVal, newVal) -> {
            if (!newVal) {
                checkRepeatPassword();
                if (!validRepeatPassword.get()) {
                    //If it is not correct, a listener is added to the text or value 
                    //so that the field is validated while it is being edited.
                    if (listenerRepeatPassword == null) {
                        listenerRepeatPassword = (a, b, c) -> checkEmail();
                        repeatPasswordField.textProperty().addListener(listenerRepeatPassword);
                    }
                }
            }
        });
    }
}