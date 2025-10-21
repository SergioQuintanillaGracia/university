package librerias.estructurasDeDatos.modelos;

/**
 * Modelo Diccionario SIN claves repetidas, o Map: Busqueda Dinamica 
 * de una Entrada de Clave dada en una Coleccion, para recuperar el 
 * Valor asociado a esta 
 * 
 */

public interface Map<C, V> {

    /** inserta la Entrada (c,v) en un Map y devuelve null; si ya
     *  existe una Entrada de Clave c en el Map, devuelve su valor 
     *  asociado y lo substituye por v (o actualiza la Entrada)
     */
    V insert(C c, V v);
    
    /** elimina la Entrada con Clave c de un Map y devuelve su 
     *  valor asociado, null si no existe una Entrada con dicha clave
     */
    V remove(C c);
    
    /** devuelve el valor asociado a la clave c en un Map,
     *  null si no existe una Entrada con dicha clave
     */
    V get(C c);
    
    /** comprueba si un Map esta vacio */
    boolean isEmpty();
    
    /** devuelve la talla, o numero de Entradas, de un Map */
    int size();
    
    /** devuelve una ListaConPI con las talla() Claves de un Map */
    ListPOI<C> keys();
}