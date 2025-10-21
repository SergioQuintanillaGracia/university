package pract7;

import java.io.IOException;
import java.net.*;
import java.util.Date;

public class ex5 {
    public static void main(String[] args) {
        try {
            DatagramSocket ds = new DatagramSocket(7777);
            DatagramPacket dp = new DatagramPacket(new byte[512], 512);

            ds.receive(dp);

            Date now = new Date();
            String nowStr = now.toString() + "\r\n";
            byte nowBytes[] = nowStr.getBytes();
            dp.setData(nowBytes);
            dp.setLength(nowBytes.length);

            ds.send(dp);

        } catch (SocketException e) {
            System.out.println("No free UDP ports to open the socket");
        } catch (IOException e) {
            System.out.println("IOException occurred");
            System.out.println(e.getMessage());
        }
    }
}
