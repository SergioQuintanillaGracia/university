import java.io.*;

public class TestPrintWriter {
    public static void main(String[] args) {
        String fileName = "file2.txt";

        try {
            PrintWriter pw = new PrintWriter(new File(fileName));
            pw.print("Test");
            pw.println("Test2");
            pw.println(2);
            pw.close();
        } catch (FileNotFoundException e) {
            System.err.println("Error opening or creating " + fileName);
        }
    }
}
