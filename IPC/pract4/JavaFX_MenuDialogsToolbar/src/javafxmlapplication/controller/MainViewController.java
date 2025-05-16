package javafxmlapplication.controller;

import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceDialog;
import javafx.scene.control.Label;
import javafx.scene.control.RadioMenuItem;
import javafx.scene.control.TextInputDialog;
import javafx.scene.control.Toggle;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.BorderPane;
import javafx.scene.web.WebView;

public class MainViewController implements Initializable {

    @FXML
    private BorderPane borderPane;
    @FXML
    private ToggleGroup buyInToggleGroup;
    @FXML
    private Button amazonButton;
    @FXML
    private Button bloggerButton;
    @FXML
    private Button ebayButton;
    @FXML
    private Button facebookButton;
    @FXML
    private Button googlePlusButton;
    @FXML
    private Label statusLabel;

    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO
    }    

    @FXML
    private void amazonButtonPressed(ActionEvent event) {
        // Get selected store option
        Toggle selectedStoreToggle = buyInToggleGroup.getSelectedToggle();
        String selectedStoreText = ((RadioMenuItem) selectedStoreToggle).getText();
        
        if (selectedStoreText.equals("Amazon")) {
            WebView webView = new WebView();
            webView.getEngine().load("https://amazon.es");
            borderPane.setCenter(webView);
        } else {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Selection error");
            alert.setHeaderText("You cannot buy in Amazon");
            alert.setContentText("Please, change the current selection in the options menu");
            alert.showAndWait();
        }
    }

    @FXML
    private void bloggerButtonPressed(ActionEvent event) {
        List<String> choices = new ArrayList<>();
        choices.add("Innersloth's Blog");
        choices.add("Gabe Newell's Blog");
        choices.add("Izanobix' Blog");
        
        ChoiceDialog<String> dialog = new ChoiceDialog<>("Izanobix' Blog", choices);
        dialog.setTitle("Select a blog");
        dialog.setHeaderText("Which blog do you want to visit?");
        dialog.setContentText("Available blogs:");
        
        Optional<String> result = dialog.showAndWait();
        if (result.isPresent()) {
            statusLabel.setText("Visiting %s".formatted(result.get()));
        }
    }

    @FXML
    private void ebayButtonPressed(ActionEvent event) {
        // Get selected store option
        Toggle selectedStoreToggle = buyInToggleGroup.getSelectedToggle();
        String selectedStoreText = ((RadioMenuItem) selectedStoreToggle).getText();
        
        if (selectedStoreText.equals("Ebay")) {
            WebView webView = new WebView();
            webView.getEngine().load("https://ebay.es");
            borderPane.setCenter(webView);
        } else {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Selection error");
            alert.setHeaderText("You cannot buy in Ebay");
            alert.setContentText("Please, change the current selection in the options menu");
            alert.showAndWait();
        }
    }

    @FXML
    private void facebookButtonPressed(ActionEvent event) {
        TextInputDialog dialog = new TextInputDialog("Dilbert");
        dialog.setTitle("Introduce your name");
        dialog.setHeaderText("Which user do you want to use to write in Facebook?");
        dialog.setContentText("Your username:");
        
        Optional<String> result = dialog.showAndWait();
        
        if (result.isPresent()) {
            statusLabel.setText("Messaging as %s".formatted(result.get()));
        }
    }

    @FXML
    private void googlePlusButtonPressed(ActionEvent event) {
        TextInputDialog dialog = new TextInputDialog("Dilbert");
        dialog.setTitle("Introduce your name");
        dialog.setHeaderText("Which user do you want to use to write in Google+?");
        dialog.setContentText("Your username:");
        
        Optional<String> result = dialog.showAndWait();
        
        if (result.isPresent()) {
            statusLabel.setText("Messaging as %s".formatted(result.get()));
        }
    }
}
