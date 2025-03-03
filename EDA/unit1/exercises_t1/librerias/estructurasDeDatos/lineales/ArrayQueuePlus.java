package librerias.estructurasDeDatos.lineales;

import librerias.estructurasDeDatos.modelos.*;

public class ArrayQueuePlus<E> extends ArrayQueue<E> implements QueuePlus<E> {
    public final int size() { 
        return super.talla;
    }
}