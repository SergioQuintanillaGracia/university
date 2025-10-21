package again.pract5;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class ex3 {
    public static void main(String[] args) {
        try {
            ServerSocket server = new ServerSocket(8080);

            while (true) {
                Socket client = server.accept();

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

                    output.printf("%s\n\r\n", line);
                }

                client.close();
            }

        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
