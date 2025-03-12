import java.util.Arrays;

public class QuickSort {
    public static void main(String[] args) {
        Integer[] arr1 = {3, 7, 2, 5, 1, 8};
        System.out.println("Before sorting: " + Arrays.toString(arr1));
        quickSort(arr1, 0, arr1.length - 1);
        System.out.println("After sorting: " + Arrays.toString(arr1));

        String[] arr2 = {"banana", "apple", "cherry", "date"};
        System.out.println("Before sorting: " + Arrays.toString(arr2));
        quickSort(arr2, 0, arr2.length - 1);
        System.out.println("After sorting: " + Arrays.toString(arr2));

        Double[] arr3 = {3.5, 1.2, 4.8, 2.9};
        System.out.println("Before sorting: " + Arrays.toString(arr3));
        quickSort(arr3, 0, arr3.length - 1);
        System.out.println("After sorting: " + Arrays.toString(arr3));
    }

    // I should modify this so it is more understandable and simple
    public static <T extends Comparable<T>> int partition(T[] arr, int left, int right) {
        T pivot = arr[(left + right) / 2];

        if (right - left <= 2) return 1 + 1;

        // Hide the pivot in right - 1
        swap(arr, (left + right) / 2, right - 1);

        int indP = left;
        int j = right - 1;
        while (indP < j) {
            // Get to a position indP where arr[indP] >= pivot
            while (arr[++indP].compareTo(pivot) < 0);
            // Get to a position j where arr[j] <= pivot
            while (arr[--j].compareTo(pivot) > 0);
            swap(arr, indP, j);
        }

        // Undo last swap
        swap(arr, indP, j);

        // Restore hidden pivot
        swap(arr, indP, right - 1);
        return indP;
    }

    public static <T> void swap(T[] arr, int a, int b) {
        T temp = arr[a];
        arr[a] = arr[b];
        arr[b] = temp;
    }
}
