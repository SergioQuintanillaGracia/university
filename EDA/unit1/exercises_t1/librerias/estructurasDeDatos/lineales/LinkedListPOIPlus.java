package librerias.estructurasDeDatos.lineales;

import librerias.estructurasDeDatos.modelos.*;

public class LinkedListPOIPlus<E>
    extends LinkedListPOI<E>
    implements ListPOIPlus<E> {

    public boolean contains(E e) {
        begin();
        while (!isEnd() && !get().equals(e)) next();
        if (isEnd()) return false;
        return true;
    }

    public boolean removeFirst(E e) {
        begin();
        while (!isEnd()) {
            if (get().equals(e)) {
                remove();
                return true;
            }
            next();
        }
        return false;
    }

    public boolean removeLast(E e) {
        LinkedNode<E> lastEPrev = null;

        begin();
        while (!isEnd()) {
            if (get().equals(e)) lastEPrev = prev;
        }

        if (lastEPrev == null) return false;

        prev = lastEPrev;
        remove();
        return true;
    }

    public boolean removeAll(E e) {
        boolean someRemoved = false;

        begin();
        while (!isEnd()) {
            if (get().equals(e)) {
                remove();
                someRemoved = true;
            } else {
                next();
            }
        }

        return someRemoved;
    }

    public void addAll(ListPOI<E> other) {
        end();
        other.begin();

        while (!other.isEnd()) {
            insert(other.get());
            other.next();
        }
    }

    public void clear() {
        begin();
        while (!isEmpty()) remove();
    }

    public void reverseFromPOI() {
        if (!isEnd()) {
            E e = get();
            remove();
            reverseFromPOI();
            insert(e);
            next();
        }
    }

    public void search(E e) {
        begin();
        while (!isEnd() && !get().equals(e)) next();
    }
}
