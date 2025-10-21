package again.pract1;

import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

public class ex1 {
    public static void main(String[] args) throws UnknownHostException, IOException {
        Socket s = new Socket("www.upv.es", 80);
        System.out.println("Connected!");
    }
}
