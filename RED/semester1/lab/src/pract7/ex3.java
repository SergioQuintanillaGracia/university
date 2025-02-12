package pract7;

import java.io.IOException;
import java.net.*;

public class ex3 {
    public static void main(String[] args) {
        try {
            DatagramSocket ds = new DatagramSocket();
            InetAddress ia = InetAddress.getByName("localhost");

            String s = "Hello\n";
            byte buf[] = s.getBytes();

            DatagramPacket dp = new DatagramPacket(buf, buf.length, ia, 7777);
            ds.send(dp);

        } catch (SocketException e) {
            System.out.println("No free UDP ports to open the socket");
        } catch (UnknownHostException e) {
            System.out.println("Couldn't resolve host");
        } catch (IOException e) {
            System.out.println("IOException occurred when sending UDP packet");
        }
    }
}
