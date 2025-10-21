package librerias.estructurasDeDatos.modelos;

public interface Queue<E> {
// metodos Modificadores del estado de una Cola:
    /** inserta el Elemento e en una Cola, o lo situa en su final 
     */
    void enqueue(E e);
    
    /** SII !esVacia(): 
     * obtiene y elimina de una Cola el Elemento que ocupa su principio 
     */
    E dequeue();
    
// metodos Consultores del estado de una Cola
    /** SII !esVacia(): 
     * obtiene el Elemento que ocupa el principio de una Cola,
     * el primero en orden de insercion 
     */
    E first();
    
    /** comprueba si una Cola esta vacia 
     */
    boolean isEmpty();
}