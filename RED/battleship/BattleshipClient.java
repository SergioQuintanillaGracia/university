import java.io.IOException;
import java.io.PrintWriter;
import java.net.NoRouteToHostException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class BattleshipClient {
    public static void main(String[] args) {
        Socket socket = null;

        try {
            // Create a socket and connect to the server
            socket = new Socket("localhost", 12345);
            System.out.println("Connected to the server");

            // Initialize the PrintWriter and Scanners
            PrintWriter pw = new PrintWriter(socket.getOutputStream(), true);
            Scanner sc = new Scanner(socket.getInputStream());
            Scanner clientSc = new Scanner(System.in);

            // Ignore the first line, which contains protocol information
            sc.nextLine();

            int hitCount = 0;

            // Client logic
            while (hitCount < 14) {
                // Get the board and display it
                System.out.println("New iteration");
                pw.println("BOARD");
                printCount(sc, 10);

                System.out.printf("Guess > ");

                String guess = clientSc.nextLine();
                pw.println("GUESS " + guess);
                printCount(sc, 1);
            }

        } catch (UnknownHostException e) {
            System.out.println("Unknown host");
        } catch (NoRouteToHostException e) {
            System.out.println("Couldn't reach the target host");
        } catch (IOException e) {
            System.out.println("Couldn't connect to the server");
        } finally {
            // Try to close the socket
            if (socket != null) {
                try {
                    socket.close();
                    System.out.println("Disconnected from the server");
                } catch (IOException e) {
                    System.out.println("Couldn't close the socket");
                }
            }
        }
    }

    public static void printCount(Scanner sc, int count) {
        while (--count > 0) {
            System.out.println(sc.nextLine());
        }
    }
}
