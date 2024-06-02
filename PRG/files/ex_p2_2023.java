import java.util.*;
import java.io.*;

public class ex_p2_2023 {
    public static void checkErrors(String fileIn, String fileErr) throws FileNotFoundException {
        Scanner input = new Scanner(new File(fileIn));
        PrintWriter errors = new PrintWriter(new File(fileErr));

        int lineNumber = 0;

        while (input.hasNext()) {
            lineNumber++;

            String data[] = input.nextLine().trim().split("([ \t])+");

            if (data.length != 3) {
                errors.printf("Error line %d: Incorrect number of data in a line.\n", lineNumber);
            } else {
                try {
                    int day = Integer.parseInt(data[0]);
                    int month = Integer.parseInt(data[1]);
                    float db = Float.parseFloat(data[2]);

                    if (db < 0) {
                        errors.printf("Error line %d: Negative value.\n", lineNumber);
                    }

                } catch (NumberFormatException e) {
                    errors.printf("Error line %d: Non-valid format.\n", lineNumber);
                }
            }
        }

        input.close();
        errors.close();
    }
}