import java.util.Scanner;
import java.io.*;

public class TestPrintWriter1 {
    public static void main(String[] args) {
        String fileName = "file1.txt";

        try {
            PrintWriter pw = new PrintWriter(new File(fileName));
            
            for (int i = 0; i < 10; i++) {
                pw.println(i);
            }

            pw.close();

            Scanner sc = new Scanner(new File(fileName));

            while (sc.hasNext()) {
                System.out.println(sc.nextLine());
            }

            sc.close();
        
        } catch (FileNotFoundException e) {
            System.err.println("Error when opening file: " + fileName);
        }
    }
}
