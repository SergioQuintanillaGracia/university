import java.io.File;

public class TestFile {
    public static void main(String[] args) {
        File f = new File("/home/sergio/Repos/University/PRG/files/file.txt");
        
        if (f.exists()) {
            System.out.println("File exists");
        } else {
            System.err.println("File doesn't exist");
        }

        System.out.println(f.getName());
        System.out.println(f.getParent());
        System.out.println(f.length());
    }
}