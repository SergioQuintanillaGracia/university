package pract7;

import java.net.InetAddress;
import java.net.UnknownHostException;

public class ex1 {
    public static void main(String[] args) {
        try {
            InetAddress ia = InetAddress.getByName(args[0]);
            System.out.println(ia.toString());

        } catch (UnknownHostException e) {
            System.out.println("Couldn't translate hostname");
        }
    }
}
