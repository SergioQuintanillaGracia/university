import java.util.Scanner;
import java.io.*;

public class ex_f2_2223 {
    public static void processMarks(String fileIn, String fileOut, String fileErr) throws FileNotFoundException {
        Scanner in = new Scanner(new File(fileIn));
        PrintWriter out = new PrintWriter(new File(fileOut));
        PrintWriter err = new PrintWriter(new File(fileErr));

        int i = 0;

        while (in.hasNext()) {
            i++;
            String line = in.nextLine();
            String data[] = line.trim().split("\t");
            
            try {
                if (data.length != 2) {
                    throw new Exception();
                }

                String name = data[0];
                double mark = Double.parseDouble(data[1]);

                mark = mark < 0 ? 0 : mark > 10 ? 10 : mark;

                out.printf("%s\t%.2f\n", name, mark);

            } catch (Exception e) {
                err.printf("Line %d: %s\n", i, line);
            }
        }

        in.close();
        out.close();
        err.close();
    }
}
