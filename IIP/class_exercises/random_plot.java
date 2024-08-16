public class random_plot {
    public static void main(String[] args) throws InterruptedException {
        int samples = 100;      // Amount of random numbers that will be generated.
        double maxNum = 1000;    // Maximum number that will be randomly generated.
        int maxBarLength = 20;  // Maximum size of the bars of the plot.
        int divisions = 20;     // Amount of bars the randomly generated values will be split into.
                                // It shouldn't be greater than maxNum.
        double arr[] = new double[samples];

        for (int i = 0; i < arr.length; i++) {
            // Fill the array with random numbers.
            arr[i] = Math.random() * maxNum;
        }

        double sampleAmount[] = new double[divisions];

        for (double i : arr) {
            // Store the amount of values that will be inside each bar of the plot.
            sampleAmount[(int) (i / maxNum * divisions)]++;
        }

        int maxSampleAmount = (int) max(sampleAmount);
        double prevRange = 0;

        // Print a bar over the plot.
        printRepeat("_", maxBarLength + 18, true);

        for (double i : sampleAmount) {
            // Calculate the ending range of the current bar. If the difference between it and the
            // maximum number that can be generated is very small, we will consider the maximum range
            // is the maximum number.
            double endingRange = prevRange + maxNum / divisions;
            if (maxNum - endingRange < 0.1) endingRange = maxNum;

            // Print the range before the current bar.
            System.out.printf("| %4d - %4d | ", (int) prevRange, (int) endingRange);
            
            prevRange = endingRange;

            // Print the bars of the appropiate size, based on the maximum sample amount, the current
            // sample amount, and the maximum length of the bars.
            int barCharCount = (int) (i / maxSampleAmount * maxBarLength);
            printRepeat("#", barCharCount, false);
            printRepeat(" ", maxBarLength - barCharCount, false);
            System.out.print(" |");

            System.out.println();
        }

        // Print a bar below the plot.
        printRepeat("_", maxBarLength + 18, true);

        // Print some properties of the stored numbers.
        System.out.println("\nSamples: %d\nAverage: %f\nMax: %f\nMin: %f\n".formatted(samples, average(arr), max(arr), min(arr)));
    }


    public static void printRepeat(String s, int amount, boolean newlineCharacter) {
        for (int j = 0; j < amount; j++) {
            System.out.print(s);
        }

        if (newlineCharacter) System.out.println();
    }


    public static double average(double arr[]) {
        double av = 0;

        for (double i : arr) {
            av += i / arr.length;
        }

        return av;
    }


    public static double max(double arr[]) {
        double max = Double.MIN_VALUE;
        
        for (double i : arr) max = i > max ? i : max;

        return max;
    }


    public static double min(double arr[]) {
        double min = Double.MAX_VALUE;

        for (double i : arr) min = i < min ? i : min;

        return min;
    }
}
