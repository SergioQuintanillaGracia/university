package pract1;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.NoRouteToHostException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class ex5 {
    public static void main(String[] args) {
        String host = "zoltar.redes.upv.es";
        int port = 7;

        try {
            System.out.printf("Connecting to %s at port %d\n", host, port);
            Socket s = new Socket(host, port);
            System.out.println("Connected");

            System.out.println("Sending line to server");
            PrintWriter pw = new PrintWriter(s.getOutputStream(), true);
            pw.println("Hello world");

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
