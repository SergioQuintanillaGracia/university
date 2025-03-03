package librerias.estructurasDeDatos.lineales;

import librerias.estructurasDeDatos.modelos.*;

public class ArrayQueue<E> implements Queue<E> {
    protected E[] elArray;    
    protected int finalC;
    protected int principioC;
    protected int talla;
    protected static final int CAPACIDAD_POR_DEFECTO = 50;
   
    /** construye una Cola vacia **/
    @SuppressWarnings("unchecked")
    public ArrayQueue() {
        elArray = (E[]) new Object[CAPACIDAD_POR_DEFECTO];
        talla = 0; 
        principioC = 0; 
        finalC = 0;
    }
    
    /** inserta el Elemento e en una Cola, o lo situa en su final **/
    //  SII no hay suficiente espacio en elArray se duplica 
    //  el tamagno de elArray circular
    public void enqueue(E e) {
        if (talla == elArray.length) duplicateCircularArray();
        elArray[finalC] = e;
        finalC = increment(finalC); 
        talla++;
    }
    
    // duplica el tamagno actual de un array circular 
    @SuppressWarnings("unchecked")
    protected void duplicateCircularArray() {
        E[] nuevoArray = (E[]) new Object[elArray.length * 2];
        for (int i = 0; i < talla; i++, principioC = increment(principioC))
            nuevoArray[i] = elArray[principioC];
        elArray = nuevoArray; 
        principioC = 0; 
        finalC = talla;
    }
    
    // incrementa un indice de un array circular 
    protected int increment(int indice) {
        if (++indice == elArray.length) indice = 0;
        return indice;
    }
    
    /** SII !esVacia(): 
     *  obtiene y elimina de una Cola el Elemento que ocupa su principio **/ 
    // principio se incrementa circularmente 
    public E dequeue() {
        E elPrimero = elArray[principioC];
        principioC = increment(principioC); 
        talla--;
        return elPrimero;
    }
    
    /** SII !esVacia(): 
     *  obtiene el Elemento que ocupa el principio de una Cola,  
     *  el primero en orden de insercion **/
    public E first() { return elArray[principioC]; }
    
    /** comprueba si una Cola esta vacia **/
    public boolean isEmpty() { return talla == 0; }
    
   /** obtiene un String con los Elementos de una Cola en orden FIFO, 
    *  o de insercion, y con el formato que se usa en el estandar de Java. 
    *  Asi, por ejemplo, si se tiene una Cola con los Integer del 1 al 4, 
    *  en orden FIFO, toString devuelve [1, 2, 3, 4]; 
    *  si la Cola esta vacia, entonces devuelve [] 
    */
    // OJO: se contempla la circularidad de elArray no solo usando el metodo
    // incrementar, sino contando el numero de Elementos desde 0 hasta talla-1
    public String toString() {
        // NOTA: se usa la clase StringBuilder, en lugar de String, 
        // por motivos de eficiencia
        StringBuilder res = new StringBuilder();
        res.append("[");
        // NOTA: por cuestiones de formato ... 
        // -Recorrer el ArrayCola 
        //  desde el primero al PENULTIMO de sus elementos; 
        // -En cada elemento visitado, 
        //  si aux es la variable del bucle de recorrido, 
        //  agnadir (append) a res aux.dato + ", "
        int aux = principioC;
        for (int i = 0, j = talla - 1; i < j; i++, aux = increment(aux))
              res.append(elArray[aux].toString() + ", ");
        // NOTA: por cuestiones de formato, al terminar el bucle, 
        // agnadir el ultimo elemento al resultado;
        // en funcion de la talla, dicho elemento es 
        // el String elArray[aux].toString()+"]" o el String "]"
        if (talla != 0) res.append(elArray[aux].toString() + "]"); 
        else res.append("]");
        return res.toString();
    }
}