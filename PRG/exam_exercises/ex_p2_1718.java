import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

public class ex_p2_1718 {
    public static File arrayToFile(int a[]) {
        File f = new File("ArrayElements.txt");
        PrintWriter pw = null;

        try {
            pw = new PrintWriter(f);
            
            for (int i : a) {
                pw.println(i);
            }

        } catch (FileNotFoundException e) {
            System.err.println("Error opening " + f);
        
        } finally {
            if (pw != null) {
                pw.close();
            }
        }

        return f;
    }
}
