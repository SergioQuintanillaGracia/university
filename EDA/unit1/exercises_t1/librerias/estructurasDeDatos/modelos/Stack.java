package librerias.estructurasDeDatos.modelos;

public interface Stack<E> {
// metodos Modificadores del estado de una Pila:
    /** inserta el Elemento e en una Pila, o lo situa en su tope 
     */
    void push(E e);
    
    /** SII !esVacia(): 
     * obtiene y elimina de una Pila el Elemento que ocupa su tope 
     */
    E pop();

// metodos Consultores del estado de una Pila:
    /** SII !esVacia(): 
     * obtiene el Elemento que ocupa el tope de una Pila 
     */
    E peek();
    
    /** comprueba si una Pila esta vacia 
     */
    boolean isEmpty();
}