package pract5;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class ex1 {
    public static void main(String[] args) {
        try {
            ServerSocket ss = new ServerSocket(7777);

            while (true) {
                Socket client = ss.accept();

                Scanner input = new Scanner(client.getInputStream());
                String line = input.nextLine();

                PrintWriter output = new PrintWriter(client.getOutputStream(), true);
                output.printf("%s\r\n", line);

                System.out.println("A client has connected to the server");
                client.close();
            }

        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
