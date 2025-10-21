package pract3;

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
        int currSize = MIN_SIZE;
        
        System.out.println("# SelectionSort. Time in microseconds.");
        System.out.println("#   Size      Average");
        System.out.println("# ---------------------");
        
        while (currSize <= MAX_SIZE) {
            long average = 0;
            
            for (int i = 0; i < REPETITIONS; i++) {
                int a[] = createArray(currSize);
                fillArrayRandom(a);
                long ti = System.nanoTime();
                MeasurableAlgorithms.selectionSort(a);
                long tf = System.nanoTime();
                long elapsed = tf - ti;
                average += elapsed / REPETITIONS;
            }
            
            System.out.println("    " + currSize + "      " + average / 1000.0);
            currSize += STEP_OF_SIZE;
        }
    }

    public static void measuringInsertionSort() { 
        int currSize = MIN_SIZE;
        
        System.out.println("# SelectionSort. Time in microseconds.");
        System.out.println("#   Size      Best     Worst     Average");
        System.out.println("# ----------------------------------------");
        
        while (currSize <= MAX_SIZE) {
            long best = Long.MAX_VALUE;
            long average = 0;
            long worst = 0;
            
            for (int i = 0; i < REPETITIONS; i++) {
                int a[] = createArray(currSize);
                fillArrayRandom(a);
                long ti = System.nanoTime();
                MeasurableAlgorithms.selectionSort(a);
                long tf = System.nanoTime();
                long elapsed = tf - ti;
                average += elapsed / REPETITIONS;
                
                if (elapsed > worst) {
                    worst = elapsed;
                }
                
                    if (elapsed < best) {
                    best = elapsed;
                }
            }
            
            System.out.println("    " + currSize + "      " + best / 1000.0 + "     " + worst / 1000.0 + "     " + average / 1000.0);
            currSize += STEP_OF_SIZE;
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
