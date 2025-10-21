package controlador;

import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;
import javafx.beans.binding.Bindings;
import javafx.collections.FXCollections;

import modelo.Persona;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.stage.Modality;
import javafx.stage.Stage;
import tarea2.controlador.VistaPersonaController;

public class VistaListaControlador implements Initializable {
    //=========================================================
    @FXML
    private ListView<Persona> personasListView;
    @FXML
    private TextField nombreTextField;
    @FXML
    private TextField apellidosTextField;
    @FXML
    private Button addButton;
    @FXML
    private Button borrarButton;

    //=========================================================
    // DEBEN conincidir los tipo del ListView y de la lista observable
    private ObservableList<Persona> datos = null; // Coleccion vinculada a la vista.
    @FXML
    private Button editButton;

    @Override
    public void initialize(URL arg0, ResourceBundle arg1) {
        // en el código de inicialización del controlador
        ArrayList<Persona> misdatos = new ArrayList<Persona>();
        misdatos.add(new Persona("Pepe", "García"));
        misdatos.add(new Persona("María", "Pérez"));
        
        //=======================================================
        // creamos la lista observable mediante un metodo de FXCollections
        datos = FXCollections.observableArrayList(misdatos);
        
        //=======================================================
        //=======================================================
        // vinculamos la lista observables de personas con el ListView
        personasListView.setItems(datos);
        
        //=======================================================
        //=======================================================
        // Hay que modificar CellFactory para mostrar el objeto Persona
        personasListView.setCellFactory((c) -> new PersonListCell());
        
        //=======================================================
        // inhabilitar botones al arrancar.
        addButton.setDisable(true);
        // si no hay selección el boton Borrar esta disabled
        borrarButton.disableProperty().bind(Bindings.equal(personasListView.getSelectionModel().selectedIndexProperty(), -1));

        // oyente de foco para el textfield.
        nombreTextField.focusedProperty().addListener((a, b, c) -> {
            // TODO Auto-generated method stub
            if (nombreTextField.isFocused()) {
                addButton.setDisable(false);
                personasListView.getSelectionModel().select(-1);
            }
        });

        // oyente de foco para el listView
        personasListView.focusedProperty().addListener((a, b, c) -> {
            if (personasListView.isFocused()) {
                addButton.setDisable(true);
            }
        });
        
        // Only enable edit button if some list item is selected
        editButton.disableProperty().bind(Bindings.equal(-1, personasListView.getSelectionModel().selectedIndexProperty()));
        
    }

    @FXML
    private void editPerson(ActionEvent event) {
        // Open modal window that allows to edit the person
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/tarea2/vista/VistaPersona.fxml"));
            Parent root = loader.load();
            
            // Pass the person to the new window's controller
            VistaPersonaController controller = loader.getController();
            controller.initPersona(personasListView.getSelectionModel().getSelectedItem());
            
            Scene scene = new Scene(root, 500, 300);
            Stage stage = new Stage();
            stage.setScene(scene);
            stage.setTitle("Editing person");
            stage.initModality(Modality.APPLICATION_MODAL);
            stage.showAndWait();
            
            if (controller.getChangesAccepted()) {
                // Update the person
                Persona p = controller.getPersona();
                datos.set(personasListView.getSelectionModel().getSelectedIndex(), p);
                System.out.println("Updated person");
            }
            
        } catch (IOException e) {
            System.out.println("Couldn't open window");
        }
    }
    
    class PersonListCell extends ListCell<Persona> {
        @Override
        protected void updateItem(Persona item, boolean empty) {
            super.updateItem(item, empty);
            
            if (item == null || empty) {
                setText(null);
            } else {
                setText(item.getNombre() + ", " + item.getApellidos());
            }
        }
    }

    @FXML
    void addAccion(ActionEvent event) {
        // añade a la colección si los campos no son vacíos y no contienen únicamente blancos
        if ((!nombreTextField.getText().isEmpty())
                && (nombreTextField.getText().trim().length() != 0)
                && (!apellidosTextField.getText().isEmpty())
                && (apellidosTextField.getText().trim().length() != 0)) {
            //============================================
            // añadimos a la lista
            datos.add(new Persona(nombreTextField.getText(), apellidosTextField.getText()));
            
            //============================================
            // vaciamos los campos despues de añadir a la lista
            nombreTextField.clear();
            apellidosTextField.clear();
            //cambio del foco al textfield inicial
            nombreTextField.requestFocus();

        }
    }

    @FXML
    void borrarAccion(ActionEvent event) {
        // borramos de la lista
        datos.remove(personasListView.getSelectionModel().getSelectedItem());
    }
}
