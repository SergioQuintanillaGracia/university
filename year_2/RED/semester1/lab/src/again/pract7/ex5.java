package again.pract7;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketTimeoutException;
import java.util.Date;

public class ex5 {
    public static void main(String[] args) {
        try (DatagramSocket ds = new DatagramSocket(7777)) {
            DatagramPacket dp = new DatagramPacket(new byte[0], 0);

            ds.setSoTimeout(5000);
            ds.receive(dp);

            byte[] dateBytes = ((new Date()).toString() + "\r\n").getBytes();

            dp.setData(dateBytes);
            // dp.setLength(dateBytes.length) is not necessary because setData calls it internally
            ds.send(dp);

        } catch (SocketTimeoutException e) {
            System.out.println("Socket timed out");

        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
