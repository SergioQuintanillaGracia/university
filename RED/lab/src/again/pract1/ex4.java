package again.pract1;

import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class ex4 {
    public static void main(String[] args) {
        int port = 13;

        try {
            InetAddress dir = InetAddress.getByName("zoltar.redes.upv.es");
            Socket s = new Socket(dir, port);
            Scanner input = new Scanner(s.getInputStream());
            System.out.println(input.nextLine());

        } catch (UnknownHostException e) {
            System.out.println("Unknown host");
        } catch (IOException e) {
            System.out.println("Couldn't connect");
        }
    }
}
