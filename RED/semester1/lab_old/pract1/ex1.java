package pract1;

import java.net.*;
import java.io.IOException;

class ex1 {
    public static void main(String[] args) throws UnknownHostException, IOException {
        String host = "www.upv.es";
        int port = 80;
        InetAddress address = InetAddress.getByName(host);

        System.out.printf("Connecting to %s at port %d\n", host, port);
        Socket s = new Socket(address, port);
        System.out.println("Connected");
    }
}
