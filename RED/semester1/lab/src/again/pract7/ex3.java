package again.pract7;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class ex3 {
    public static void main(String[] args) {
        try {
            DatagramSocket ds = new DatagramSocket();

            byte[] buff = "Hello\n".getBytes();
            InetAddress ia = InetAddress.getByName("localhost");

            DatagramPacket dp = new DatagramPacket(buff, buff.length, ia, 7777);

            ds.send(dp);
            ds.close();

        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
