/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javafxmlapplication;

import java.net.URL;
import java.util.ResourceBundle;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.ColorPicker;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.ToggleButton;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import javafx.scene.shape.Circle;
import static javafxmlapplication.Utils.*;

/**
 *
 * @author jsoler
 */
public class FXMLDocumentController implements Initializable {

    @FXML
    private Circle circle;
    @FXML
    private GridPane gridpane;
    
    private int rowCount;
    private int colCount;
    
    private double dragStartX;
    private double dragStartY;
    
    private Paint fillColor;
    private boolean isCircleHidden = false;
    
    @FXML
    private ToggleButton toggleButton;
    @FXML
    private Slider sizeSlider;
    @FXML
    private ColorPicker colorPicker;
    
    //=========================================================
    // you must initialize here all related with the object 
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        fillColor = circle.getFill();
        circle.setOnKeyPressed(this::handleKeyPressed);
        gridpane.setOnMousePressed(this::handleMousePressed);
        circle.setOnMousePressed(this::handleMousePressedBall);
        circle.setOnMouseDragged(this:: handleMouseDraggedBall);
        circle.setOnMouseReleased(this::handleMouseReleased);
        rowCount = gridpane.getRowCount();
        colCount = gridpane.getColumnCount();
        sizeSlider.setValue(circle.getRadius());
        circle.radiusProperty().bind(sizeSlider.valueProperty());
        colorPicker.valueProperty().addListener(this::changeFillColor);
        colorPicker.setValue((Color) circle.getFill());
    }
    
    private void changeFillColor(ObservableValue<? extends Color> obs, Color oldColor, Color newColor) {
        fillColor = newColor;
        if (!isCircleHidden) {
            circle.setFill(newColor);
        }
    }
    
    private int normalize(int num, int limit) {
        return (num + limit) % limit;
    }
    
    private void moveCircleToCellAt(double x, double y) {
        int row = Math.max(0, Math.min(rowCount - 1, rowCalc(gridpane, y)));
        int col = Math.max(0, Math.min(colCount - 1, columnCalc(gridpane, x)));
        
        gridpane.getChildren().remove(circle);
        gridpane.add(circle, col, row);
    }
    
    public void handleMousePressedBall(MouseEvent event) {
        dragStartX =  event.getSceneX();
        dragStartY = event.getSceneY();
    }
    
    public void handleMouseDraggedBall(MouseEvent event) {
        circle.setTranslateX(event.getSceneX() - dragStartX);
        circle.setTranslateY(event.getSceneY() - dragStartY);
    }
    
    public void handleMouseReleased(MouseEvent event) {
        circle.setTranslateX(0);
        circle.setTranslateY(0);
        moveCircleToCellAt(event.getSceneX(), event.getSceneY());
        event.consume();
    }
    
    public void handleMousePressed(MouseEvent event) {
        double x = event.getSceneX();
        double y = event.getSceneY();
        
        moveCircleToCellAt(x, y);
    }
    
    public void handleKeyPressed(KeyEvent event) {
        if (event.getCode() == KeyCode.UP) {
            System.out.println("moving up");
            int row = GridPane.getRowIndex(circle);
            int newRow = normalize(row - 1, rowCount);
            GridPane.setRowIndex(circle, newRow);
        }
        
        if (event.getCode() == KeyCode.DOWN) {
            System.out.println("moving down");
            int row = GridPane.getRowIndex(circle);
            int newRow = normalize(row + 1, rowCount);
            GridPane.setRowIndex(circle, newRow);
        }
        
        if (event.getCode() == KeyCode.LEFT) {
            System.out.println("moving left");
            int col = GridPane.getColumnIndex(circle);
            int newCol = normalize(col - 1, colCount);
            GridPane.setColumnIndex(circle, newCol);
        }
        
        if (event.getCode() == KeyCode.RIGHT) {
            System.out.println("moving right");
            int col = GridPane.getColumnIndex(circle);
            int newCol = normalize(col + 1, colCount);
            GridPane.setColumnIndex(circle, newCol);
        }
    }

    @FXML
    private void toggleButtonPressed(ActionEvent event) {
        if (toggleButton.isSelected()) {
            fillColor = circle.getFill();
            circle.setFill(Color.TRANSPARENT);
            isCircleHidden = true;
        } else {
            circle.setFill(fillColor);
            isCircleHidden = false;
        }
    }
}
