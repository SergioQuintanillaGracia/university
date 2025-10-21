import java.io.File;

public class TestFile {
    public static void main(String[] args) {
        try {
            test();
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println("hi");
        }
    }

    public static void test() {
        int arr[] = new int[5];

        arr[10] = 2;
    }
}