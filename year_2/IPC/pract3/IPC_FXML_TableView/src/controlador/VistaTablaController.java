/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package controlador;

import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TableCell;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.ComboBoxListCell;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.stage.Modality;
import javafx.stage.Stage;
import modelo.Persona;

/**
 * FXML Controller class
 *
 * @author jsoler
 */
public class VistaTablaController implements Initializable {

    private ObservableList<Persona> datos = null; // Colecci�n vinculada a la vista.

    @FXML
    private Button addButton;
    @FXML
    private Button modificarButton;
    @FXML
    private Button borrarButton;
    @FXML
    private TableColumn<Persona, String> nombreColumn;
    @FXML
    private TableColumn<Persona, String> apellidosColumn;
    @FXML
    private TableView<Persona> personasTableV;
    @FXML
    private TableColumn<Persona, String> imagenColumn;

    private void inicializaModelo() {
        ArrayList<Persona> misdatos = new ArrayList<Persona>();
        misdatos.add(new Persona("Pepe", "García"));
        misdatos.add(new Persona("María", "Pérez"));
        //==================================================
        // create ObservableList, FXCollections
        datos = FXCollections.observableArrayList(misdatos);
    }

    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        inicializaModelo();
        //=================================================
        // connect model with tableView setItems()
        personasTableV.setItems(datos);
        
        //=================================================
        // initialize tableColumn   setCellValueFactory()
        nombreColumn.setCellValueFactory(personRow -> new SimpleStringProperty(personRow.getValue().getNombre()));
        apellidosColumn.setCellValueFactory(personRow -> new SimpleStringProperty(personRow.getValue().getApellidos()));
        imagenColumn.setCellFactory(personRow -> new ImageTableCell());
        imagenColumn.setCellValueFactory(personRow -> new SimpleStringProperty(personRow.getValue().getImageRoute()));

        //===============================================
        // add bindings for diable buttons
        
    }

    @FXML
    private void addAction(ActionEvent event) {
        // Open modal window that allows to add a person
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/vista/VistaPersona.fxml"));
            Parent root = loader.load();
            
            VistaPersonaController controller = loader.getController();
            controller.initPersona(new Persona("", ""));
            
            Scene scene = new Scene(root, 500, 300);
            Stage stage = new Stage();
            stage.setScene(scene);
            stage.setTitle("Adding person");
            stage.initModality(Modality.APPLICATION_MODAL);
            stage.showAndWait();

            // Add the person
            Persona p = controller.getPersona();
            datos.add(p);
            System.out.println("Added person");
            
        } catch (IOException e) {
            System.out.println("Couldn't open window");
        }
    }

    @FXML
    private void updateAction(ActionEvent event) {
        // Open modal window that allows to edit a person
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("/vista/VistaPersona.fxml"));
            Parent root = loader.load();
            
            // Pass the person to the new window's controller
            VistaPersonaController controller = loader.getController();
            controller.initPersona(personasTableV.getSelectionModel().getSelectedItem());
            
            Scene scene = new Scene(root, 500, 300);
            Stage stage = new Stage();
            stage.setScene(scene);
            stage.setTitle("Editing person");
            stage.initModality(Modality.APPLICATION_MODAL);
            stage.showAndWait();
            
            if (controller.getChangesAccepted()) {
                // Update the person
                Persona p = controller.getPersona();
                datos.set(personasTableV.getSelectionModel().getSelectedIndex(), p);
                System.out.println("Updated person");
            }
            
        } catch (IOException e) {
            System.out.println("Couldn't open window");
        }
    }

    @FXML
    void deleteAction(ActionEvent event) {
        // borramos de la lista
        datos.remove(personasTableV.getSelectionModel().getSelectedItem());
    }
    
    class ImageTableCell extends TableCell<Persona, String> {
        private ImageView view = new ImageView();
        private Image image;
        
        @Override
        protected void updateItem(String imageRoute, boolean isEmpty) {
            if (imageRoute == null || isEmpty) {
                setText(null);
                setGraphic(null);
            } else {
                image = new Image(imageRoute, 25, 25, true, true);
                view.setImage(image);
                setGraphic(view);
            }
        }
    }
}
