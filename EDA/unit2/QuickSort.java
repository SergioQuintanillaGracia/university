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

        // Test case 1: Integer array
        Integer[] intArr = {3, 1, 4, 1, 5, 9, 2, 6, 5};
        System.out.println("Test 1: " + quickSelect(intArr, 3)); // Expect the 3rd smallest element

        // Test case 2: String array
        String[] strArr = {"apple", "orange", "banana", "grape", "pear"};
        System.out.println("Test 2: " + quickSelect(strArr, 2)); // Expect the 2nd smallest string lexicographically

        // Test case 3: Double array
        Double[] doubleArr = {2.5, 3.1, 4.7, 1.2, 8.6};
        System.out.println("Test 3: " + quickSelect(doubleArr, 4)); // Expect the 4th smallest element

        // Test case 4: Edge case - Single element
        Integer[] singleElementArr = {42};
        System.out.println("Test 4: " + quickSelect(singleElementArr, 1)); // Expect 42

        // Test case 5: Edge case - Already sorted array
        Integer[] sortedArr = {1, 2, 3, 4, 5};
        System.out.println("Test 5: " + quickSelect(sortedArr, 3)); // Expect 3
    }

    public static <T extends Comparable<T>> T quickSelect(T[] arr, int k) {
        int left = 0;
        int right = arr.length - 1;
        int pivotIndex = partition(arr, left, right);;
        
        while (pivotIndex + 1 != k) {
            if (pivotIndex + 1 > k) {
                right = pivotIndex - 1;
            } else if (pivotIndex + 1 < k) {
                left = pivotIndex + 1;
            }
            pivotIndex = partition(arr, left, right);
        }

        return arr[pivotIndex];
    }

    public static <T extends Comparable<T>> void quickSort(T[] arr, int left, int right) {
        if (left < right) {
            // The array has more than 2 elements
            int pivot = partition(arr, left, right);
            quickSort(arr, left, pivot - 1);
            quickSort(arr, pivot + 1, right);
        }
    }

    public static <T extends Comparable<T>> int partition(T[] arr, int left, int right) {
        // Choose the pivot
        T pivot = arr[right];

        // Index of last value smaller than the pivot
        int i = left - 1;

        // Traverse arr[left..right] and move all smaller elements to the left
        for (int j = left; j <= right - 1; j++) {
            if (arr[j].compareTo(pivot) < 0) {
                // If the current element is smaller than the pivot, add it to the left
                // part of the array, next to i
                i++;
                swap(arr, i, j);
            }
        }

        // The pivot should be just after i, which is the last value smaller than the pivot
        // Remember the pivot is the last element of the array, at the right
        swap(arr, i + 1, right);
        // Return the position of the pivot
        return i + 1;
    }

    public static <T> void swap(T[] arr, int a, int b) {
        T temp = arr[a];
        arr[a] = arr[b];
        arr[b] = temp;
    }
}
