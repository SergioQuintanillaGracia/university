package pract7;

import java.io.IOException;
import java.net.*;

public class ex4 {
    public static void main(String[] args) {
        try {
            DatagramSocket ds = new DatagramSocket();
            InetAddress ia = InetAddress.getByName("localhost");

            String s = "Hello\n";
            byte buf[] = s.getBytes();

            DatagramPacket dp = new DatagramPacket(buf, buf.length, ia, 7777);
            ds.send(dp);

            // Create a new buffer for receiving the packet, as if we don't do it, the maximum length of the received
            // packet will be the length of the buffer for the sent packet
            dp.setData(new byte[512]);
            dp.setLength(512);

            ds.receive(dp);
            String receivedStr = new String(dp.getData(), 0, dp.getLength());
            System.out.println(receivedStr);

        } catch (SocketException e) {
            System.out.println("No free UDP ports to open the socket");
        } catch (UnknownHostException e) {
            System.out.println("Couldn't resolve host");
        } catch (IOException e) {
            System.out.println("IOException occurred when sending UDP packet");
        }
    }
}
