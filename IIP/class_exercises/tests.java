public class tests {
    public static void main(String[] args) {
        int arr[] = new int[]{1, 2, 3, 4,5};

        doubleArr(arr);

        for (int i : arr) System.out.print(i + " ");
        System.out.println();
    }


    public static void doubleArr(int arr[]) {
        for (int i = 0; i < arr.length; i++) {
            arr[i] *= 2;
        }
    }
}
