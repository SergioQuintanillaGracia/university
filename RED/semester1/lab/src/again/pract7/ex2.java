package again.pract7;

import java.io.IOException;
import java.net.DatagramSocket;

public class ex2 {
    public static void main(String[] args) {
        try {
            DatagramSocket server = new DatagramSocket();
            System.out.printf("Port: %d\n", server.getLocalPort());

        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
