package modelo;


import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

public class Persona {
	
	private final StringProperty nombre = new SimpleStringProperty();
	private final StringProperty apellidos = new SimpleStringProperty();
        private final StringProperty imageRoute = new SimpleStringProperty();
		
	public Persona(String nombre, String apellidos) {
		this.nombre.setValue(nombre);
		this.apellidos.setValue(apellidos);
                imageRoute.setValue(null);
	}
        
        public Persona(String nombre, String apellidos, String imageRoute) {
            this(nombre, apellidos);
            this.imageRoute.setValue(imageRoute);
        }
	
	public  StringProperty NombreProperty() {
		return this.nombre;
	}
	public String getNombre() {
		return this.NombreProperty().get();
	}
	public final void setNombre(String Nombre) {
		this.NombreProperty().set(Nombre);
	}
	public  StringProperty ApellidosProperty() {
		return this.apellidos;
	}
	public String getApellidos() {
		return this.ApellidosProperty().get();
	}
	public  void setApellidos(String Apellidos) {
		this.ApellidosProperty().set(Apellidos);
	}
        public String getImageRoute() {
            return imageRoute.get();
        }
        public void setImageRoute(String newImageRoute) {
            imageRoute.set(newImageRoute);
        }
}