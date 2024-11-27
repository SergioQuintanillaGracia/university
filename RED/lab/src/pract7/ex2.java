package pract7;

import java.net.DatagramSocket;
import java.net.SocketException;

public class ex2 {
    public static void main(String[] args) {
        try {
            DatagramSocket ds = new DatagramSocket();
            System.out.println("Datagram socket was assigned port " + ds.getLocalPort());

        } catch (SocketException e) {
            System.out.println("No free UDP sockets left");
        }
    }
}
