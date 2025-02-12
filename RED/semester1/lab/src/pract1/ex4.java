package pract1;

import java.net.*;
import java.io.IOException;
import java.util.Scanner;

class ex4 {
    public static void main(String[] args) {
        String host = "zoltar.redes.upv.es";
        int port = 13;

        try {
            System.out.printf("Connecting to %s at port %d\n", host, port);
            Socket s = new Socket(host, port);
            System.out.println("Connected");

            Scanner sc = new Scanner(s.getInputStream());
            String serverLine = sc.nextLine();
            System.out.printf("Returned by the server: %s\n", serverLine);
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
