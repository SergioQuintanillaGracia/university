package again.pract7;

import java.net.InetAddress;
import java.net.UnknownHostException;

public class ex1 {
    public static void main(String[] args) {
        String hostname = args[0];

        try {
            InetAddress ia = InetAddress.getByName(hostname);
            System.out.printf(ia.toString());

        } catch (UnknownHostException e) {
            System.out.println("No IP for that host");
        }
    }
}
