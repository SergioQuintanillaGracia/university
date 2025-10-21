package librerias.estructurasDeDatos.modelos;

public interface List<E> {
// metodos Modificadores del estado de la Lista:
    /** SII 0<=i<=talla(): 
     * inserta el elemento e en la posicion i de una Lista 
     */
    void insert(E e, int i);
    
    /** SII 0<=i<talla(): 
     * elimina el elemento que ocupa la posicion i de una Lista 
     */
    void remove(int i);
    
// metodos Consultores del estado de la Lista:
    /** SII 0<=i<talla(): 
     * devuelve el elemento que ocupa la posici�n ide una Lista 
     */
    E get(int i);
    
    /** comprueba si una Lista esta vacia 
     */
    boolean isEmpty();
    
    /** devuelve la talla de una Lista, o su numero de elementos 
     */
    int size();
}