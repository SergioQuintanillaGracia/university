package again.pract6;

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
                Service s = new Service(client);
                s.start();
            }

        } catch (IOException e) {
            System.out.println(e);
        }
    }
}

class Service extends Thread {
    static int id = 0;
    Socket client;

    public Service(Socket client) {
        this.client = client;
    }

    @Override
    public void run() {
        try {
            System.out.printf("Thread %d started\n", id++);

            Scanner input = new Scanner(client.getInputStream());
            PrintWriter output = new PrintWriter(client.getOutputStream(), true);

            while (true) {
                String line = input.nextLine();
                if (line.equals("END")) break;
                output.printf("%s\r\n", line);
            }

            client.close();

        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
