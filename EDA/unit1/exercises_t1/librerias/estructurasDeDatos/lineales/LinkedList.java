package librerias.estructurasDeDatos.lineales;

import librerias.estructurasDeDatos.modelos.*;

public class LinkedList<E> implements List<E> {

    protected LinkedNode<E> primero;
    protected int talla;

    /** construye una Lista vacia, de talla cero **/
    public LinkedList() {
        primero = null;
        talla = 0;
    }

    /**
     * SII 0<=i<=talla():
     * inserta el elemento e en la posicion i de una Lista
     */
    public void insert(E e, int i) {
        LinkedNode<E> nuevo = new LinkedNode<E>(e);
        talla++;
        LinkedNode<E> aux = primero;
        LinkedNode<E> ant = null;
        for (int j = 0; j < i; j++) {
            ant = aux;
            aux = aux.next;
        }
        nuevo.next = aux;
        if (ant == null) primero = nuevo;
        else ant.next = nuevo;
    }

    /**
     * SII talla()>0 AND 0<=i<talla():
     * elimina el elemento que ocupa la posicion i de una Lista
     */
    public void remove(int i) {
        talla--;
        LinkedNode<E> aux = primero;
        LinkedNode<E> ant = null;
        for (int j = 0; j < i; j++) {
            ant = aux;
            aux = aux.next;
        }
        if (ant == null) primero = aux.next;
        else ant.next = aux.next;
    }

    /**
     * SII talla()>0 AND 0<=i<talla():
     * devuelve el elemento que ocupa la i-esima posicion de una Lista
     */
    public E get(int i) {
        LinkedNode<E> aux;
        int j;
        for (aux = primero, j = 0; j < i; aux = aux.next, j++);
        return aux.data;
    }

    /** comprueba si una Lista esta vacia **/
    public boolean isEmpty() {
        return primero == null;
        // alternativamente: return talla == 0;
    }

    /** devuelve la talla de una Lista **/
    public int size() {
        return this.talla;
    }

    /**
     * devuelve el String con los elementos de una Lista en orden de insercion
     * y con el formato que se usa en el estandar de Java
     * Asi, por ejemplo, si se tiene una Lista con los Integer del 1 al 4, en
     * orden de inserci�n, toString devuelve [1, 2, 3, 4];
     * si la Lista esta vacia, entonces devuelve []
     */
    public String toString() {
        StringBuilder res = new StringBuilder();
        res.append("[");
        if (talla == 0) return res.append("]").toString();
        LinkedNode<E> aux = primero;
        for (int i = 0, j = talla - 1; i < j; i++, aux = aux.next) res.append(
            aux.data.toString() + ", "
        );
        res.append(aux.data.toString() + "]");
        return res.toString();
    }
}
