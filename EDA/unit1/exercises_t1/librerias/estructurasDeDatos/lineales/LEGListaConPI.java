package librerias.estructurasDeDatos.lineales;

import librerias.estructurasDeDatos.modelos.ListPOI;

/**
 * Implementa la interfaz ListaConPI mediante una LEG ...
 * (a) Con Nodo ficticio cabecera.
 * (b) Una referencia al primer Nodo.
 * (c) Una referencia al ultimo Nodo.
 * (d) Para representar el Punto de Interes, una referencia al Nodo
 * anterior al que ocupa el punto de interes.
 * (e) Un int talla que representa la talla de la LEG.
 *
 * @version Febrero 2019
 * @param <E> tipo de datos de la estructura
 */

public class LEGListaConPI<E> implements ListPOI<E> {

    protected LinkedNode<E> pri, ant, ult;
    protected int talla;

    /** construye una Lista Con PI vacia **/
    public LEGListaConPI() {
        pri = ult = ant = new LinkedNode<E>(null);
        talla = 0;
    }

    /**
     * inserta e en una Lista antes del Elemento que ocupa su PI,
     * que permanece inalterado.
     *
     * @param e Elemento a insertar.
     **/
    public void insert(E e) {
        LinkedNode<E> nuevo = new LinkedNode<E>(e, ant.next);
        ant.next = nuevo;
        if (nuevo.next == null)
            ult = nuevo;
        ant = ant.next;
        talla++;
    }

    /**
     * SII !esFin(): elimina de una Lista el Elemento que ocupa
     * su PI que permanece inalterado.
     **/
    public void remove() {
        talla--;
        if (ant.next == ult)
            ult = ant;
        ant.next = ant.next.next;
    }

    /**
     * situa el PI de una Lista en su inicio, sobre su primer
     * elemento si no esta vacia.
     **/
    public void begin() {
        ant = pri;
    }

    /**
     * SII !esFin(): situa sobre el siguiente Elemento el PI
     * de una Lista.
     **/
    public void next() {
        ant = ant.next;
    }

    /**
     * situa el PI de una Lista en su fin, detras de su ultimo
     * Elemento si no esta vacia.
     **/
    public void end() {
        ant = ult;
    }

    /**
     * SII !esFin(): obtiene el Elemento que ocupa el PI de
     * una Lista.
     *
     * @return E, el Elemento que ocupa el PI de una Lista.
     */
    public E get() {
        return ant.next.data;
    }

    /**
     * comprueba si el PI de una Lista esta en su fin.
     *
     * @return true si el PI de una Lista esta en su fin y
     *         false en caso contrario
     **/
    public boolean isEnd() {
        return ant == ult;
    }

    /**
     * comprueba si una Lista Con PI esta vacia
     *
     * @return true si una Lista Con PI esta vacia y
     *         false en caso contrario
     **/
    public boolean isEmpty() {
        return pri == ult;
    }

    /**
     * devuelve la talla de una Lista Con PI, i.e. su
     * numero de elementos.
     *
     * @return int, el numero de Elementos de una Lista con PI.
     **/
    public int size() {
        return talla;
    }

    /**
     * devuelve el String con los Elementos de una Lista con PI
     * en orden de insercion.
     *
     * @return String que contiene los Elementos de una Lista con
     *         PI, en el mismo formato que usa el estandar de Java para
     *         los arrays.
     **/
    public String toString() {
        StringBuilder s = new StringBuilder();
        s.append("[");
        LinkedNode<E> aux = pri.next;
        for (int i = 1; i < talla; i++, aux = aux.next) {
            s.append(aux.data.toString() + ", ");
        }
        if (talla != 0) {
            s.append(aux.data.toString() + "]");
        } else {
            s.append("]");
        }
        return s.toString();
    }
}
