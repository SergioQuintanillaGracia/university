import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.InputMismatchException;
import java.util.Scanner;

public class ex_f2_1617 {
    public static void filterNonRealNumbers(String fileIn) {
        Scanner sc = null;
        PrintWriter pw = null;
        String fileOut = fileIn + "_new";

        try {
            sc = new Scanner(new File(fileIn));
            pw = new PrintWriter(new File(fileOut));

            while (sc.hasNext()) {
                try {
                    double d = sc.nextDouble();
                    pw.println(d);
                
                } catch (InputMismatchException e) {
                    sc.nextLine();
                }
            }
        
        } catch (FileNotFoundException e) {}

        finally {
            if (sc != null) {
                sc.close();
            }
            
            if (pw != null) {
                pw.close();
            }
        }
    }
}
