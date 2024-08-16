import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

public class ex_f2_1819 {
    public static void processData(String fileIn, String fileOut) throws FileNotFoundException {
        Scanner sc = new Scanner(new File(fileIn));
        PrintWriter pw = new PrintWriter(new File(fileOut));
        int line = 0;

        while (sc.hasNext()) {
            line++;

            String data[] = sc.nextLine().split("([ \t])+");

            try {
                if (data.length != 4) {
                    throw new Exception("Unexpected number of columns.");
                }

                String name = data[0];
                int age = Integer.parseInt(data[1]);
                int points = Integer.parseInt(data[2]);
                int num = Integer.parseInt(data[3]);

                if (age < 0 || points < 0 || num < 0) {
                    throw new Exception("Negative value.");
                }
            
            } catch (NumberFormatException e) {
                pw.println("Error line " + line + ": Invalid format for an integer.");
            } catch (Exception e) {
                pw.println("Error line " + line + ": " + e.getMessage());
            }
        }

        sc.close();
        pw.close();
    }
}
