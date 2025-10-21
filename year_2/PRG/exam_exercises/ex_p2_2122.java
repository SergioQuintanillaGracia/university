import java.util.InputMismatchException;
import java.util.Scanner;
import java.io.*;

public class ex_p2_2122 {
    public static void readWriteDouble(Scanner kbd, String fileOut) {
        PrintWriter pw = null;

        try {
            pw = new PrintWriter(new File(fileOut));

            while (kbd.hasNext()) {
                try {
                    double d = kbd.nextDouble();

                    if (d == 0) {
                        pw.close();
                        return;
                    }

                    pw.println(d);
                
                } catch (InputMismatchException e) {
                    kbd.nextLine();
                }
            }
        } catch (FileNotFoundException e) {
            System.out.println("Couldn't open " + fileOut);
        
        } finally {
            if (pw != null) {
                pw.close();
            }
        }
    }
}
