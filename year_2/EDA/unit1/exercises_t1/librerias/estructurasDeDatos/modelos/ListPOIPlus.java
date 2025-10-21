package librerias.estructurasDeDatos.modelos;

public interface ListPOIPlus<E> extends ListPOI<E> {
    
    /** comprueba si el Elemento e esta en una Lista Con PI **/
    boolean contains(E e);    
    
    /** elimina la primera aparicion del Elemento e en una Lista Con PI 
     *  y devuelve true, o devuelve false si e no esta en la Lista
     */
    boolean removeFirst(E e);
    
    /** si el Elemento e esta en una Lista Con PI elimina su ultima aparicion
     *  y devuelve true, o devuelve false si e no esta en la Lista 
     */
    boolean removeLast(E e);
    
    /** si el Elemento e esta en una Lista Con PI elimina todas sus apariciones,
     *  y devuelve true, o devuelve false si e no esta en la Lista 
     */
    boolean removeAll(E e);
    
    /** elimina todos los Elementos de una Lista Con PI **/
    void clear();
    
    /** concatena una Lista Con PI con otra **/
    void concatenate(ListPOI<E> otra);
    
    /** desplaza todos los elementos de la lista una posici�n hacia la derecha,
     *  de forma que el �ltimo elemento deber� pasar a ser el primero
     */
    void moveToRight();

    /** desplaza todos los elementos de la lista una posici�n hacia la izquierda,
     *  de forma que el primer elemento deber� pasar a ser el �ltimo 
     */
    void moveToLeft();

    /** invierte in-situ una Lista a partir de su PI **/
    void invertFromPOI();
}