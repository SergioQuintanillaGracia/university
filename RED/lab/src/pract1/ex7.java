package pract1;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.NoRouteToHostException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class ex7 {
    public static void main(String[] args) {
        String host = "smtp.upv.es";
        int port = 25;

        try {
            System.out.printf("Connecting to %s at port %d\n", host, port);
            Socket s = new Socket(host, port);
            System.out.println("Connected");

            Scanner sc = new Scanner(s.getInputStream());
            System.out.printf("First server line: %s\n", sc.nextLine());

            PrintWriter pw = new PrintWriter(s.getOutputStream(), true);
            pw.println("HELLO from my laptop...");

            System.out.printf("Second server line: %s\n", sc.nextLine());

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
