package librerias.estructurasDeDatos.lineales

public class LinkedStackPlus<E> extends LinkedStack<E> {
    public E base() {
        LinkedNode<E> base = top;
        while (top.next != null)
            base = top.next;
        return base.data;
    }

    public E baseMethodsRecursive() {
        E res;
        E el = pop();
        if (isEmpty())
            res = el;
        else
            res = baseMethodsRecursive();
        push(el);
        return res;
    }
}
