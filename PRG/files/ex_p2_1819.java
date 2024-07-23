import java.util.*;
import java.io.*;

public class ex_p2_1819 {
    public static void sumInt(String fileIn, String fileOut) throws FileNotFoundException {
        Scanner sc = new Scanner(new File(fileIn));
        PrintWriter pw = new PrintWriter(new File(fileOut));
        int sum = 0;

        while (sc.hasNext()) {
            try {
                int val = sc.nextInt();
                sum += val;
            } catch (InputMismatchException e) {
                pw.println("(Error: " + sc.next() + ")");
            }
        }

        pw.println("Sum: " + sum);

        sc.close();
        pw.close();
    }
}
