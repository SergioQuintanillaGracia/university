package pract1;

import java.io.IOException;
import java.net.NoRouteToHostException;
import java.net.Socket;
import java.net.UnknownHostException;

public class ex9 {
    public static void main(String[] args) {
        String host = "www.upv.es";
        int port = 80;

        try {
            System.out.printf("Connecting to %s at port %d\n", host, port);
            Socket s = new Socket(host, port);
            System.out.println("Connected");

            System.out.println("\nConnection information:");
            System.out.printf("Port: %d\n", s.getPort());
            System.out.printf("Inet address: %s\n", s.getInetAddress());
            System.out.printf("Local port: %d\n", s.getLocalPort());
            System.out.printf("Local address: %s\n", s.getLocalAddress());

            s.close();
        } catch (UnknownHostException e) {
            System.out.println("Unknown host");
        } catch (NoRouteToHostException e) {
            System.out.println("Couldn't reach the target host");
        } catch (IOException e) {
            System.out.println("Couldn't connect to the server");
        }
    }
}
