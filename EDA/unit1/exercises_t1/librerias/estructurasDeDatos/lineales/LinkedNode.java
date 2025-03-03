package librerias.estructurasDeDatos.lineales;

/**
 * Clase friendly que representa un Nodo de una LEG
 * TIENE UN:
 * - dato, el elemento que contiene el Nodo
 * - siguiente, la referencia al siguiente Nodo de la LEG
 *
 */

class LinkedNode<E> {
    E data;
    LinkedNode<E> next;

    /**
     * Crea un Nodo que contiene al Elemento e y al que sigue el Nodo s
     *
     * @param e Elemento que contiene un Nodo
     * @param s Nodo siguiente a un Nodo
     */
    LinkedNode(E e, LinkedNode<E> s) {
        data = e;
        next = s;
    }

    /**
     * Crea un Nodo que contiene al Elemento e y al que no sigue ninguno
     *
     * @param e Elemento que contiene un Nodo
     */
    LinkedNode(E data) {
        this(data, null);
    }
}
