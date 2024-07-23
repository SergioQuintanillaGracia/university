import java.util.*;
import java.io.*;

public class ex_f2_2122 {
    public static double sumNum(String fileIn, String fileOut) throws FileNotFoundException {
        Scanner sc = new Scanner(new File(fileIn)).useLocale(Locale.US);
        PrintWriter pw = new PrintWriter(new File(fileOut));
        double sum = 0;
        int words = 0;

        while (sc.hasNext()) {
            try {
                double d = sc.nextDouble();
                sum += d;
            } catch (InputMismatchException e) {
                pw.println(sc.next());
                words++;
            }
        }

        pw.println(words + " words");
        pw.close();
        return sum;
    }
}
