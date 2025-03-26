/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package javafxmlapplication;

/**
 *
 * @author jsole
 */
import javafx.beans.property.StringProperty;
import javafx.scene.control.TextInputControl;

import java.lang.reflect.Field;
import java.util.List;

public class ListenerChecker {

    public static boolean hasTextListeners(TextInputControl control) {
        try {
            StringProperty textProperty = control.textProperty();
            Field listenersField = textProperty.getClass().getDeclaredField("changeListeners");
            listenersField.setAccessible(true);
            List<?> listeners = (List<?>) listenersField.get(textProperty);
            return listeners != null && !listeners.isEmpty();
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }
}

