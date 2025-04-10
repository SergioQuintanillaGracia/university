/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tarea2.controlador;

import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.TextField;
import modelo.Persona;

/**
 * FXML Controller class
 *
 * @author jsoler
 */
public class VistaPersonaController implements Initializable {

    @FXML
    private TextField nombreTextField;
    @FXML
    private TextField apellidosTextField;
     boolean pulsadoOK;


    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        
        pulsadoOK = false;
        persona = new Persona("", "");
    }    

    
    private Persona persona;
    
    public void initPersona( Persona p)
    {
    persona = p;
    nombreTextField.setText(p.getNombre());
    apellidosTextField.setText(p.getApellidos());
    }
    
    
    @FXML
    private void aceptar(ActionEvent event) {
        // añade a la colección si los campos no son vacíos y no contienen únicamente blancos
        if ((!nombreTextField.getText().isEmpty())
                && (nombreTextField.getText().trim().length() != 0)
                && (!apellidosTextField.getText().isEmpty())
                && (apellidosTextField.getText().trim().length() != 0)) {
            //============================================
            // añadimos a la lista

            persona.setNombre(nombreTextField.getText());
            persona.setApellidos(apellidosTextField.getText());
            
            //============================================
            // vaciamos los campos despues de añadir a la lista
            nombreTextField.clear();
            apellidosTextField.clear();
            pulsadoOK = true;
            
            
        nombreTextField.getScene().getWindow().hide();
    }
        
    }   
        public Persona getPersona(){
        return persona; 

        }
        
        
        public boolean isOKPressed( )
        {
        return pulsadoOK;
        }
        
    @FXML
    private void cancelar(ActionEvent event) {
        nombreTextField.getScene().getWindow().hide();
        
        pulsadoOK = false;
    }


    
}
