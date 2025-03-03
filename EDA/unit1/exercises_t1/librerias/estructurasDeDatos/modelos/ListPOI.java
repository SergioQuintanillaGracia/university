package librerias.estructurasDeDatos.modelos;

/**
 * Modelo de una Lista Con Punto de Interes, o de Acceso Secuencial 
 * a los Elementos de una Coleccion
 *  
 */

public interface ListPOI<E> {
// metodos Modificadores del estado de la Lista Con PI:
    /** inserta e en una Lista antes del Elemento que ocupa su PI, 
     * que permanece inalterado 
     */
    void insert(E e);
    
    /** SII !esFin(): 
     * elimina de una Lista el Elemento que ocupa su PI, 
     * que permanece inalterado 
     */
    void remove();
    
// metodos Modificadores del estado del PI de la Lista:
    /** situa el PI de una Lista en su inicio **/
    void begin();
    /** SII !esFin(): avanza el PI de una Lista **/
    void next();
    /** situa el PI de una Lista en su fin **/
    void end();
    
// metodos Consultores del estado de la Lista Con PI:
    /** SII !esFin(): obtiene el Elemento que ocupa el PI de una Lista **/
    E get();
    /** comprueba si el PI de una Lista esta en su fin **/
    boolean isEnd();
    /** comprueba si una Lista Con PI esta vacia **/
    boolean isEmpty();
    /** devuelve la talla de una Lista, o su numero de elementos **/
    int size();
}