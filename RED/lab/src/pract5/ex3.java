package pract5;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class ex3 {
    public static void main(String[] args) {
        try {
            ServerSocket ss = new ServerSocket(8080);

            while (true) {
                Socket client = ss.accept();
                System.out.println("Client connected");
                Scanner input = new Scanner(client.getInputStream());
                PrintWriter output = new PrintWriter(client.getOutputStream(), true);

                output.printf("HTTP/1.0 200 OK\r\n");
                output.printf("Content-Type: text/plain\r\n");
                output.printf("\r\n");

                while (input.hasNextLine()) {
                    String line = input.nextLine();

                    if (line.isEmpty()) {
                        output.printf("\r\n");
                        break;
                    }

                    output.printf("%s\r\n", line);
                }

                System.out.println("Finished sending response");
                client.close();
            }

        } catch (IOException e) {
            System.err.println(e);
        }
    }
}
