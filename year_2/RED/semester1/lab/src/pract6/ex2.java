package pract6;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class ex2 {
    public static void main(String[] args) throws IOException {
        Socket client = new Socket("zoltar.redes.upv.es", 7788);
        PrintWriter output = new PrintWriter(client.getOutputStream(), true);
        Scanner sc = new Scanner(System.in);

        // Start reading messages received from the server
        ServiceRead sr = new ServiceRead(client);
        sr.start();

        while (sc.hasNextLine()) {
            // Get input messages and send them to the server
            String line = sc.nextLine();

            if (line.equalsIgnoreCase("quit")) break;
            output.printf("%s\r\n", line);
        }

        client.close();
        sc.close();
    }
}


class ServiceRead extends Thread {
    Socket client;

    public ServiceRead(Socket s) {
        client = s;
    }

    @Override
    public void run() {
        try {
            Scanner input = new Scanner(client.getInputStream());

            // Read lines received and print them
            while (input.hasNextLine()) {
                String line = input.nextLine();

                System.out.println("# " + line);
            }

        } catch (IOException e) {
            throw new RuntimeException(e);
        } finally {
            try {
                client.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }
}