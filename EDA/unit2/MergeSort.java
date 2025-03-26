import java.util.Arrays;

public class MergeSort {
    public static void main(String[] args) {
        MergeSort sorter = new MergeSort();

        Integer[] arr1 = {5, 3, 8, 4, 2, 7, 1, 6};
        sorter.mergeSort(arr1, 0, arr1.length - 1);
        System.out.println("Sorted: " + Arrays.toString(arr1));
        
        String[] arr2 = {"banana", "apple", "cherry", "date"};
        sorter.mergeSort(arr2, 0, arr2.length - 1);
        System.out.println("Sorted: " + Arrays.toString(arr2));
        
        Double[] arr3 = {3.5, 2.2, 1.1, 4.8, 5.0};
        sorter.mergeSort(arr3, 0, arr3.length - 1);
        System.out.println("Sorted: " + Arrays.toString(arr3));
    }

    public <T extends Comparable<T>> void mergeSort(T[] arr, int left, int right) {
        if (right > left) {
            int middle = (left + right) / 2;
            mergeSort(arr, left, middle);
            mergeSort(arr, middle + 1, right);
            merge(arr, left, middle, right);
        }
    }

    public <T extends Comparable<T>> void merge(T[] arr, int left, int middle, int right) {
        int leftSize = middle - left + 1;
        int rightSize = right - middle;

        T[] leftArr = Arrays.copyOfRange(arr, left, middle + 1);
        T[] rightArr = Arrays.copyOfRange(arr, middle + 1, right + 1);

        int i = 0;
        int j = 0;
        int k = left;

        while (i < leftSize && j < rightSize) {
            if (leftArr[i].compareTo(rightArr[j]) <= 0) {
                arr[k++] = leftArr[i++];
            } else {
                arr[k++] = rightArr[j++];
            }
        }

        while (i < leftSize) arr[k++] = leftArr[i++];
        while (j < rightSize) arr[k++] = rightArr[j++];
    }
}