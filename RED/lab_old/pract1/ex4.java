package pract1;

import java.net.*;
import java.io.IOException;

class ex3 {
    public static void main(String[] args) {
        String host = "zoltar.redes.upv.es";
        int port = 13;

        try {
            System.out.printf("Connecting to %s at port %d\n", host, port);
            Socket s = new Socket(host, port);
            System.out.println("onnected");
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