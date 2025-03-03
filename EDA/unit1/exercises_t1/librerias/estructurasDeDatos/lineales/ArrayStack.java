import librerias.estructurasDeDatos.modelos.Stack;

public class ArrayStack<E> implements Stack<E> {
    E[] arr;
    int top;
    final int DEFAULT_CAPACITY = 50;

    @SuppressWarnings("unchecked")
    public ArrayStack() {
        arr = (E[]) new Object[DEFAULT_CAPACITY];
        top = -1;
    }

    @SuppressWarnings("unchecked")
    private void doubleArraySize() {
        E[] newArr = (E[]) new Object[arr.length * 2];
        System.arraycopy(arr, 0, newArr, 0, top);
        arr = newArr;
    }

    public void push(E el) {
        if (top == arr.length - 1) {
            doubleArraySize();
        }

        arr[++top] = el;
    }

    public E pop() {
        return arr[top--];
    }

    public boolean isEmpty() {
        return top == -1;
    }

    public E peek() {
        return arr[top];
    }
}
