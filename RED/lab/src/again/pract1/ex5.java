package again.pract1;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class ex5 {
    public static void main(String[] args) {
        int port = 7;

        try {
            InetAddress dir = InetAddress.getByName("zoltar.redes.upv.es");
            Socket s = new Socket(dir, port);
            Scanner input = new Scanner(s.getInputStream());
            PrintWriter output = new PrintWriter(s.getOutputStream(), true);
            output.printf("Hello world\r\n");
            System.out.println(input.nextLine());

        } catch (UnknownHostException e) {
            System.out.println("Unknown host");
        } catch (IOException e) {
            System.out.println("Couldn't connect");
        }
    }
}
