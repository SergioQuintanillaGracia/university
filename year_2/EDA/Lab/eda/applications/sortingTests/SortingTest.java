package applications.sortingTests;

import libraries.util.Sorting;
import java.util.Arrays;
import java.util.Locale;

/**
 * The SortingTest class can test and time the methods to
 * sort generic arrays defined in the libraries.util.Sorting
 * class.
 *
 *  @author (EDA)
 *  @version (Curso 2019-2020)
 */

public class SortingTest {

    /**
     * Checks the correctness of the mergeSort2 method
     * of the Sorting class, knowing that the quickSort
     * method is correct.
     */
    public static boolean check() {
        Integer[] a1 = createRandomInteger(100000);
        Integer[] a2 = Arrays.copyOf(a1, a1.length);

        // To be completed by the student:
        // check that mergeSort2 works properly,
        // using the areEqual method from the
        // Sorting class to compare its result
        // against quickSort

        // Sort a1 using quickSort:
        Sorting.quickSort(a1);

        // Sort a2 using mergeSort (version 2):
        Sorting.mergeSort2(a2);

        // Are a1 (quickSort) and a2 (mergeSort2) equal?
        return Sorting.areEqual(a1, a2);
    }

    /**
     * Prints a table of timings of the time it takes
     * mergeSort1, mergeSort2 and quickSort to sort
     * arrays of integers.
     */
    public static void time() {
        final int BEGIN = 10000; // Smallest size of array to be tested
        final int END = 100000; // Largest array tested
        final int INC = BEGIN; // Size increment
        final int numRep = 200; // Number of repetitions per size
        double t1, t2; // These hold the start and end times
        double acc1, acc2, acc3; // Accumulators for the three sorting methods
        Integer[] aux1, aux2, aux3; // Arrays to be filled with random numbers and sorted

        System.out.println("#----------------------------------------------");
        System.out.println("# Comparison between quickSort and mergeSort: ");
        System.out.println("# Timings in milliseconds for Integers.");
        System.out.println("#----------------------------------------------");
        System.out.println("#  Size     mergeSort1   mergeSort2   quickSort");
        System.out.println("#----------------------------------------------");
        for (int k = BEGIN; k <= END; k = k + INC) {
            int size = k;
            t1 = 0; t2 = 0;
            acc1 = 0; acc2 = 0; acc3 = 0;
            for (int i = 1; i <= numRep; i++) {
                aux1 = createRandomInteger(size);
                aux2 = Arrays.copyOf(aux1, aux1.length);
                aux3 = Arrays.copyOf(aux1, aux1.length);

                t1 = System.nanoTime();
                Sorting.mergeSort1(aux1);
                t2 = System.nanoTime();
                acc1 += t2 - t1;

                t1 = System.nanoTime();
                Sorting.mergeSort2(aux2);
                t2 = System.nanoTime();
                acc2 += t2 - t1;

                t1 = System.nanoTime();
                Sorting.quickSort(aux3);
                t2 = System.nanoTime();
                acc3 += t2 - t1;
            }

            System.out.printf(Locale.US,
                              "%1$8d %2$12.4f %3$12.4f %4$12.4f\n",
                              size,
                              acc1 / numRep / 1e6,
                              acc2 / numRep / 1e6,
                              acc3 / numRep / 1e6);
        }
    }

    /**
     * Returns an array of size Integers generated randomly.
     *
     * @param size  Length of the resulting array
     * @return Integer[]
     */
    public static Integer[] createRandomInteger(int size) {
        Integer[] aux = new Integer[size];
        for (int i = 0; i < aux.length; i++) {
            aux[i] = (int) (Math.random() * (10 * size));
        }
        return aux;
    }

    /**
     * Prints a table of timings of the time it takes
     * mergeSort1, mergeSort2 and quickSort to sort
     * arrays of strings.
     */
    public static void timeString() {
        final int BEGIN = 10000; // Smallest size of array to be tested
        final int END = 100000; // Largest array tested
        final int INC = BEGIN; // Size increment
        final int numRep = 200; // Number of repetitions per size
        double t1, t2; // These hold the start and end times
        double acc1, acc2, acc3; // Accumulators for the three sorting methods
        String[] aux1, aux2, aux3; // Arrays to be filled with random Strings and sorted
        final int prefixLength = 50; // Length of the prefix that is common to all strings

        System.out.println("#----------------------------------------------");
        System.out.println("# Comparison between quickSort and mergeSort: ");
        System.out.println("# Timings in milliseconds for Strings - " + prefixLength);
        System.out.println("#----------------------------------------------");
        System.out.println("#  Size     mergeSort1   mergeSort2   quickSort");
        System.out.println("#----------------------------------------------");
        for (int k = BEGIN; k <= END; k = k + INC) {
            int size = k;
            t1 = 0; t2 = 0;
            acc1 = 0; acc2 = 0; acc3 = 0;
            for (int i = 1; i <= numRep; i++) {
                aux1 = createRandomString(size, prefixLength);
                aux2 = Arrays.copyOf(aux1, aux1.length);
                aux3 = Arrays.copyOf(aux1, aux1.length);

                t1 = System.nanoTime();
                Sorting.mergeSort1(aux1);
                t2 = System.nanoTime();
                acc1 += t2 - t1;

                t1 = System.nanoTime();
                Sorting.mergeSort2(aux2);
                t2 = System.nanoTime();
                acc2 += t2 - t1;

                t1 = System.nanoTime();
                Sorting.quickSort(aux3);
                t2 = System.nanoTime();
                acc3 += t2 - t1;
            }

            System.out.printf(Locale.US,
                              "%1$8d %2$12.4f %3$12.4f %4$12.4f\n",
                              size,
                              acc1 / numRep / 1e6,
                              acc2 / numRep / 1e6,
                              acc3 / numRep / 1e6);
        }
    }

    /**
     * Returns an array of size Strings generated randomly.
     * All share the same prefix of length n.
     *
     * @param size  Length of the resulting array
     * @param n Length of the prefix shared by all strings
     * @return String[]
     */
    public static String[] createRandomString(int size, int n) {
        String[] arr = new String[size];
        StringGenerator g = new StringGenerator(50);
        
        for (int i = 0; i < size; i++) {
            arr[i] = g.generate();
        }
        
        return arr;
    }

    public static void main(String[] args) {
        boolean okMS2 = check();
        if (okMS2) {
            System.out.println("To obtain timings for Integers, you must execute time()");
            System.out.println("To obtain timings for Strings, you must execute timeString()");
        }
        else {
            System.out.println("ERROR in mergeSort2: it doesn't sort arrays properly\n" +
                    "IT ISN'T POSSIBLE to time the algorithms until it is fixed.");
        }
    }
}

