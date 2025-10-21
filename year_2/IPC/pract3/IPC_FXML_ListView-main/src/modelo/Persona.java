package modelo;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

public class Persona {

    private final StringProperty nombre = new SimpleStringProperty();
    private final StringProperty apellidos = new SimpleStringProperty();

    public Persona(String nombre, String apellidos) {
        this.nombre.setValue(nombre);
        this.apellidos.setValue(apellidos);
    }

    public final StringProperty NombreProperty() {
        return this.nombre;
    }

    public String getNombre() {
        return this.nombre.get();
    }

    public void setNombre(String Nombre) {
        this.nombre.set(Nombre);
    }

    public final StringProperty ApellidosProperty() {
        return this.apellidos;
    }

    public final java.lang.String getApellidos() {
        return this.apellidos.get();
    }

    public final void setApellidos(String Apellidos) {
        this.apellidos.set(Apellidos);
    }

}
