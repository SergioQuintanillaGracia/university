import java.io.File;
import java.io.FileNotFoundException;
import java.util.InputMismatchException;
import java.util.Scanner;

public class ex_f2_1718 {
    public static StackIntLinked fileToStack(String fileIn) {
        StackIntLinked s = new StackIntLinked();
        Scanner sc = null;

        try {
            sc = new Scanner(new File(fileIn));

            while (sc.hasNext()) {
                try {
                    int num = sc.nextInt();
                    s.push(num);
                
                } catch (InputMismatchException e) {
                    sc.nextLine();
                }
            }
        
        } catch (FileNotFoundException e) {
            System.err.println("File not found");
        
        } finally {
            if (sc != null) {
                sc.close();
            }
        }

        return s;
    }
}
