import java.util.Scanner;
import java.io.*;

public class TestScanner {
    public static void main(String[] args) {
        Scanner sc = null;

        try {
            sc = new Scanner(new File("things.txt"));
        } catch (FileNotFoundException e) {
            System.err.println("File doesn't exist. " + e.getMessage());
            System.exit(0);
        }

        int n1 = sc.nextInt();
        int n2 = sc.nextInt();
        int n3 = sc.nextInt();
        int n4 = sc.nextInt();
        sc.nextLine();
        String line = sc.nextLine();

        System.out.println("Numbers are: %d, %d, %d, %d".formatted(n1, n2, n3, n4));
        System.out.println("Line is: %s".formatted(line));

        sc.close();
    }
}
