package pract3_2;

import java.util.Locale;
import java.util.Random;

/** Class MeasuringSortingAlgorithms: Empirical analysis of the temporal cost of sorting algorithms
 *  @author PRG - ETSInf
 *  @version Curso 2023-24
 */
class MeasuringSortingAlgorithms {
    // Constants to be used when varying the measuring parameters
    public static final int MIN_SIZE = 1000, 
                            MAX_SIZE = 10000;
    public static final int STEP_OF_SIZE = 1000,
                            REPETITIONS = 200;
    public static final double NMS = 1e3;  // ratio microseconds/nanoseconds


    private static final Random generator = new Random(); // Generator of random numbers

  
    /* Creates an array of length size 
     * @param size int, size of the array
     * @result int[], created array
     */
    private static int[] createArray(int size) { 
        int[] a = new int[size];
        return a;
    }
  
    /* Fills an int array with random values
     * @param a int[], array to be filled
     */
    private static void fillArrayRandom(int[] a) {
        for (int i = 0; i < a.length; i++) {
            a[i] = generator.nextInt();
        }
    }

    /* Fills an int array sorted in ascending order
     * @param a int[], array to be filled
     */
    private static void fillArraySortedInAscendingOrder(int[] a) { 
        for (int i = 0; i < a.length; i++) {
            a[i] = i;
        }
    }

    /* Fills an int array sorted in descending order
     * @param a int[], array to be filled
     */
    private static void fillArraySortedInDescendingOrder(int[] a) {
        for (int i = 0; i < a.length; i++) {
            a[i] = a.length - i;
        }
    }

    public static void measuringSelectionSort() {
        System.out.println("# SelectionSort: Time in microseconds");
        System.out.println("#   Size         Average");
        System.out.println("# ------------------------");
        
        for (int i = MIN_SIZE; i <= MAX_SIZE; i += STEP_OF_SIZE) {
            int a[] = createArray(i);
            long average = 0;
            
            for (int j = 0; j < REPETITIONS; j++) {
                fillArrayRandom(a);
                
                long ti = System.nanoTime();
                MeasurableAlgorithms.selectionSort(a);
                long tf = System.nanoTime();
                long elapsed = tf - ti;
                
                average += elapsed / REPETITIONS;
            }
            
            System.out.println(String.format("   %5d %13.3f", i, average / 1000.0));
        }
    }

    public static void measuringInsertionSort() { 
        System.out.println("# InsertionSort: Time in microseconds");
        System.out.println("#   Size        Best        Worst       Average");
        System.out.println("# -----------------------------------------------");
        
        for (int i = MIN_SIZE; i <= MAX_SIZE; i += STEP_OF_SIZE) {
            int a[] = createArray(i);
            long worst = 0;
            long best = 0;
            long average = 0;
            
            for (int j = 0; j < REPETITIONS; j++) {
                fillArrayRandom(a);
                
                long ti = System.nanoTime();
                MeasurableAlgorithms.insertionSort(a);
                long tf = System.nanoTime();
                long elapsed = tf - ti;
                
                average += elapsed / REPETITIONS;
            }
            
            for (int j = 0; j < REPETITIONS; j++) {
                fillArraySortedInAscendingOrder(a);
                
                long ti = System.nanoTime();
                MeasurableAlgorithms.insertionSort(a);
                long tf = System.nanoTime();
                long elapsed = tf - ti;
                
                best += elapsed / REPETITIONS;
            }
            
            for (int j = 0; j < REPETITIONS; j++) {
                fillArraySortedInDescendingOrder(a);
                
                long ti = System.nanoTime();
                MeasurableAlgorithms.insertionSort(a);
                long tf = System.nanoTime();
                long elapsed = tf - ti;
                
                worst += elapsed / REPETITIONS;
            }
            
            System.out.println(String.format("  %5d %13.3f %13.3f %13.3f", i, best / 1000.0, worst / 1000.0, average / 1000.0));
        }
    }
  
    private static void help() {
        System.out.println("Usage: java MeasurigSortingAlgorithms <algorithm_number>");
        System.out.println("   Where <algorithm_number> can be: ");
        System.out.println("   1 -> Selection Sort");
        System.out.println("   2 -> Insertion Sort");
    }

    public static void main(String[] args) {
        if (args.length != 1) { help(); }
        else {
            try {
                int a = Integer.parseInt(args[0]);
                switch (a) {
                    case 1: 
                        measuringSelectionSort(); 
                        break;
                    case 2: 
                        measuringInsertionSort(); 
                        break;
                    default: 
                        help();
                }
            } catch (Exception e) { help(); }
        }
    }
}
