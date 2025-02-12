package pract6;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class ex1 {
    public static void main(String[] args) throws IOException {
        ServerSocket server = new ServerSocket(7777);

        while (true) {
            Socket client = server.accept();
            Service s = new Service(client);
            s.start();
        }
    }
}


class Service extends Thread {
    static int id = 0;

    Socket client;

    public Service(Socket s) {
        client = s;
    }

    @Override
    public void run() {
        try {
            System.out.printf("Thread %d started\n", id++);

            Scanner input = new Scanner(client.getInputStream());
            PrintWriter output = new PrintWriter(client.getOutputStream(), true);

            while (input.hasNextLine()) {
                String line = input.nextLine();

                if (line.equalsIgnoreCase("end")) break;

                output.println(line);
            }

            input.close();

        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}