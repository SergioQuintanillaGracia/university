package again.pract5;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class ex1 {
    public static void main(String[] args) {
        try {
            ServerSocket server = new ServerSocket(7777);

            while (true) {
                Socket client = server.accept();

                System.out.println("A client has connected");

                Scanner input = new Scanner(client.getInputStream());
                PrintWriter output = new PrintWriter(client.getOutputStream(), true);

                output.printf("%s\r\n", input.nextLine());

                client.close();
            }

        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
