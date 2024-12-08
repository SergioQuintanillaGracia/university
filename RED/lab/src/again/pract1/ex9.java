package again.pract1;

import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class ex9 {
    public static void main(String[] args) {
        int port = 80;
        String addr = "zoltar.redes.upv.es";

        try {
            InetAddress dir = InetAddress.getByName(addr);
            Socket s = new Socket(dir, port);
            System.out.printf("Local addr: %s\nRemote addr: %s\nLocal port: %s\nRemote port: %s\n",
                    s.getLocalAddress(), s.getInetAddress(), s.getLocalPort(), s.getPort());

        } catch (UnknownHostException e) {
            System.out.println("Unknown host");
        } catch (IOException e) {
            System.out.println("Couldn't connect");
        }
    }
}
