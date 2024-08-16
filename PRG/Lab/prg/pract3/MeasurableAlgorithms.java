package pract3;

/** Class MeasurableAlgorithms: class with the methods to be analyzed. It is a tool class.
 *  @author PRG - ETSInf
 *  @version Curso 2023-24
 */
public class MeasurableAlgorithms {
    /** Linear Search
     *  @param a int[], array of int
     *  @param e int, value to look for
     *  @return int, position of e in a or -1 if e is not in a
     */
    public static int linearSearch(int[] a, int e) {
        int i = 0;
        while(i < a.length && (a[i] != e)) { i++; }
        if (i < a.length) { return i; }
        else              { return -1; }
    }

    /** Selection Sort
     *  @param a int[], array of int
     */
    public static void selectionSort(int[] a) {
        int posMin, temp;
        for(int i = 0; i < a.length - 1; i++) {
            posMin = i;
            for(int j = i + 1; j < a.length; j++) {
                if (a[j] < a[posMin]) { posMin = j; }
            }
            temp = a[posMin];
            a[posMin] = a[i];
            a[i] = temp;
        }
    }

    /** InsertionSort
     *  @param a int[], array of int
     */
    public static void insertionSort(int[] a) {
        int temp;
        for(int i = 1; i < a.length; i++) {
            int j = i - 1;
            temp = a[i];
            while(j >= 0 && a[j] > temp) {
                a[j + 1] = a[j];
                j--;
            }
            a[j + 1] = temp;
        }
    }
}
