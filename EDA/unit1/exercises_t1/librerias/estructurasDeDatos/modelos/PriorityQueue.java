package librerias.estructurasDeDatos.modelos;

/**
 * Modelo de una Cola de Prioridad, o de Busqueda Dinamica 
 * del Elemento de maxima prioridad en una Coleccion
 * 
 */

public interface PriorityQueue<E extends Comparable<E>> {
// metodos Modificadores del estado de una Cola de Prioridad (CP):
    /** atendiendo a su prioridad, 
     *  inserta el Elemento e en una Cola de Prioridad 
     */
    void  insert(E e);
    
    /** SII !esVacia(): 
     *  obtiene y elimina el Elemento con maxima prioridad de una CP 
     */
    E  removeMin();
    
// metodos Consultores del estado de una Cola de Prioridad (CP):
    /** SII !esVacia(): 
     *  obtiene el Elemento con maxima prioridad de una CP 
     */
    E  getMin();
    
    /** comprueba si una CP esta vac�a 
     */
    boolean isEmpty();
}