package pract5;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class ex2 {
    public static void main(String[] args) {
        try {
            ServerSocket ss = new ServerSocket(7777);

            while (true) {
                Socket client = ss.accept();

                System.out.println("Client connected");
                System.out.printf("Server address: %s:%d | Client address: %s:%d\n",
                        ss.getInetAddress(), ss.getLocalPort(), client.getInetAddress(), client.getPort());

                Scanner input = new Scanner(client.getInputStream());
                String line = input.nextLine();

                PrintWriter output = new PrintWriter(client.getOutputStream(), true);
                output.printf("%s\r\n", line);

                client.close();
            }

        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
