package again.pract7;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class ex4 {
    public static void main(String[] args) {
        try {
            DatagramSocket ds = new DatagramSocket();
            byte[] data = "hello\n".getBytes();
            DatagramPacket p = new DatagramPacket(data, data.length, InetAddress.getByName("localhost"), 7777);
            ds.send(p);

            p.setData(new byte[512]);
            ds.receive(p);
            String decodedData = new String(p.getData(), 0, p.getLength());
            System.out.printf("Received: %s\n", decodedData);

        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
