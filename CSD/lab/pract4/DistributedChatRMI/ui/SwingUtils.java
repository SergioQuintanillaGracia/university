//
// Studends can safely ignore this file
// It just contains some simple Swing helpers
//

package ui;

import java.awt.Container;
import java.awt.Dimension;
import java.beans.PropertyChangeSupport;
import java.util.function.Consumer;
import javax.swing.DefaultListModel;
import javax.swing.JComponent;
import javax.swing.JTextField;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.ListDataEvent;
import javax.swing.event.ListDataListener;

//
// Studends can safely ignore this file
// It just contains some simple Swing helpers
//


/**
 * SwingUtils
 * 
 * <p>A bunch of swing classes that help building swing User Interfaces.
 * 
 * <p>Generic and not bound to any specific program.
 * 
 * @author Pablo Galdamez
 */
public class SwingUtils {
    
    /**
     * Simple observable to allow registration on data changes.
     * 
     * @param <T> type of data for the observable.
     */
    
    public static class MyObservable<T> {
        private T data;
        private final PropertyChangeSupport support;

        public MyObservable (T data) {
            this.data = data;
            this.support = new PropertyChangeSupport(this);
        }
        
        public void setWithoutNotification (T data) {
            this.data = data; 
        }

        public void set (T newData) {
            this.data = newData; 
            this.support.firePropertyChange("data", null, newData);
        }

        public T get () { return data; }

        @SuppressWarnings("unchecked")
        public void onChanged (Consumer<T> cb) {
            support.addPropertyChangeListener ( ev -> cb.accept( (T) ev.getNewValue()) );
        }        
    }
    
    

    /**
     * Simple swing document listener to update an Observalbe automatically when 
     * JTextField changes.
     * 
     * <p> It works both directions: when the observable changes, it also reflects 
     * changes into JTextField content.
     */
    
    public static class MyDocumentListener implements DocumentListener {
        
        // String model where user wants to keep data
        private final MyObservable<String> strModel; 
        
        // JTextField to monitor
        private final JTextField field; 
        
        /**
         * Build a Document listener binding a JTextField to an String observable.
         * 
         * @param field JTextField
         * @param strModel String observable
         */
        public MyDocumentListener (JTextField field, MyObservable<String> strModel) {
            this.field = field;
            this.strModel = strModel;
            
            // On Observable changes, update the JTextField
            strModel.onChanged( val-> field.setText(val) );
            
            // Initialize text field with model data
            String value = strModel.get();            
            if (value != null) field.setText(value);
        }        
        
        @Override
        public void insertUpdate(DocumentEvent e) { updateModel(); }        
        @Override
        public void removeUpdate(DocumentEvent e) { updateModel(); }
        @Override
        public void changedUpdate(DocumentEvent e) { updateModel(); }
        
        protected void updateModel() {
            // Get the JTextField value
            String fieldVal = field.getText();
            
            // If different than the Observable, update it.
            if (!field.getText().equals (strModel.get())) {
                
                // Set value without notification to avoid cycles.
                strModel.setWithoutNotification (fieldVal); 
            }
        }
    };
    
    
    /**
     * Simple extension to DefaultListModel to include a simple functional listener
     * 
     * @param <T> Type of data for the list model elements
     */
    public static class MyListModel<T> extends DefaultListModel<T> {

        private MyObservable <Integer> lastAdition = new MyObservable<> (null);

        public MyListModel () {
            this.addListDataListener(new ListDataListener () {
                
                @Override
                public void intervalAdded(ListDataEvent e) {
                    lastAdition.set(e.getIndex0());
                }
                
                @Override
                public void intervalRemoved(ListDataEvent e) {}

                @Override
                public void contentsChanged(ListDataEvent e) {}
                
            });
        }
        public void onAdition (Consumer<Integer> cb) {
            lastAdition.onChanged(cb);
        }
    }

    /**
     * Fix a Swing component size. On container resize, size won't change.
     * 
     * @param comp component whose size we want immutable
     * 
     * @return same component.
     */
    public static JComponent setFixedSize (JComponent comp) {
        Dimension bs = comp.getSize();
        comp.setPreferredSize(bs);
        comp.setMinimumSize(bs);
        comp.setMaximumSize(bs);
        return comp;
    }

    /**
     * Set the minimum size acceptable for a Swing component
     * 
     * @param comp Component whose minimum size we want to limit
     * @param min Minimum size
     * @return Same component
     */
    public static Container setMinimumSize (Container comp, Dimension min) {
        comp.setPreferredSize(min);
        comp.setMinimumSize(min);
        return comp;
    }
    
}
