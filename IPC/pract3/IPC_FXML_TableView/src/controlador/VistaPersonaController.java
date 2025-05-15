/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package controlador;

import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.TextField;
import javafx.scene.control.cell.ComboBoxListCell;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
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
    @FXML
    private Button aceptarButton;
    @FXML
    private ComboBox<?> imagesCombo;

    private Persona persona;
    private boolean changesAccepted = false;
    
    public void initPersona(Persona p) {
        persona = p;
        nombreTextField.setText(p.getNombre());
        apellidosTextField.setText(p.getApellidos());
    }
    
    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO
        //=================================================================
        // add 3 path images to ComboBox
        
        
        //=================================================================
        // change the way the ComboBox cell is visualized, setCellFactory
        
        //=================================================================
        // change the way the ComboBox selecction is visualized, setButtonCell
        
        
        //==================================================================
        // add binding with the disabled property on aceptarButton
        
        
    }

    public boolean getChangesAccepted() {
        return changesAccepted;
    }
    
    public Persona getPersona() {
        return persona;
    }

    @FXML
    private void aceptar(ActionEvent event) {
        changesAccepted = true;
        persona.setNombre(nombreTextField.getText());
        persona.setApellidos(apellidosTextField.getText());
        nombreTextField.getScene().getWindow().hide();
    }

    @FXML
    private void cancelar(ActionEvent event) {
        nombreTextField.getScene().getWindow().hide();
    }

    //=======================================================
    // class to display images in the combobox
    // to us with setCellFactory and setButtonCell
    class ImageComboCell extends ComboBoxListCell<String> {

        private ImageView view = new ImageView();
        private Image imagen;

        @Override
        public void updateItem(String t, boolean bln) {
            super.updateItem(t, bln);
            if (t == null || bln) {
                setText(null);
                setGraphic(null);
            } else {
                imagen = new Image(t, 25, 25, true, true);
                view.setImage(imagen);
                setGraphic(view);
                setText(null);
            }
        }
    }

}
