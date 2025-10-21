import java.io.IOException;
import java.io.PrintWriter;
import java.net.NoRouteToHostException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;
import java.util.InputMismatchException;
import java.util.Arrays;
import java.util.List;

public class BattleshipClient {
    public static void main(String[] args) {
        int maxHits = 14;

        Socket socket = null;
        PrintWriter pw = null;
        Scanner serverSc = null;
        Scanner clientSc = null;

        try {
            // Create a socket and connect to the server
            socket = new Socket("localhost", 12345);
            System.out.println("Connected to the server");

            // Initialize the PrintWriter and Scanners
            pw = new PrintWriter(socket.getOutputStream(), true);
            serverSc = new Scanner(socket.getInputStream());
            clientSc = new Scanner(System.in);

            // Ignore the first line, which contains protocol information
            serverSc.nextLine();

            boolean continuePlaying = true;
            int hitCount;

            // Client logic
            while (continuePlaying) {
                System.out.print("\n\n");

                // Reset the board in case this is not the first game
                pw.println("RESET");
                // Ignore the "OK" line returned by the server
                serverSc.nextLine();

                // Initialize / reset hitCount to 0
                hitCount = 0;

                // Get and print the board for the first time
                printBoard(pw, serverSc);

                while (hitCount < maxHits) {
                    System.out.printf("Game progress: %d/%d (%d%%)\n", hitCount, maxHits,
                                      (int) ((double) hitCount / maxHits * 100));
                    System.out.printf("Guess > ");

                    if (handleGuess(pw, clientSc, serverSc)) {
                        // Get a guess by the user
                        // `handleGuess()` returns true if a boat was hit
                        hitCount++;
                    }

                    // Get and print the new state of the board
                    printBoard(pw, serverSc);
                }

                // The game has ended
                // Ask the player if they want to continue playing
                continuePlaying = getPlayAgainInput(clientSc);
            }

        } catch (UnknownHostException e) {
            System.out.println("Unknown host");
        
        } catch (NoRouteToHostException e) {
            System.out.println("Couldn't reach the target host");
        
        } catch (IOException e) {
            System.out.println("Couldn't connect to the server");
        
        } finally {
            // Close PrintWriter and Scanners
            if (pw != null) pw.close();
            if (serverSc != null) serverSc.close();
            if (clientSc != null) clientSc.close();

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

    public static void printCount(Scanner serverSc, int count) {
        while (count-- > 0) {
            System.out.println(serverSc.nextLine());
        }
    }

    public static void printBoard(PrintWriter pw, Scanner serverSc) {
        pw.println("BOARD");

        // Leave space on the left for the letters
        System.out.printf("  ");

        // Print top number row
        for (int i = 1; i <= 10; i++) {
            System.out.printf("%d ", i);
        }

        System.out.println();

        // Print the board lines preceded by their corresponding letter
        for (int i = 65; i <= 74; i++) {
            System.out.printf("%c ", i);
            printCount(serverSc, 1);
        }
    }

    public static String getGuess(Scanner clientSc) {
        String guess = clientSc.nextLine();

        if (guess.length() == 2 || guess.length() == 3) {
            // Check if the first character is a letter
            if (Character.isLetter(guess.charAt(0))) {
                // Check if the remaining characters are digits
                try {
                    Integer.parseInt(guess.substring(1, guess.length()));
                    return guess.toUpperCase();
                } catch (NumberFormatException e) { }
            }
        }

        throw new InputMismatchException("Invalid guess");
    }

    /*
     * Returns `true` if a boat is hit, and `false` otherwise.
     */
    public static boolean handleGuess(PrintWriter pw, Scanner clientSc, Scanner serverSc) {
        boolean boatHit = false;

        try {
            // Get the user guess and send it to the server
            String guess = getGuess(clientSc);
            pw.println("GUESS " + guess);
            
            // React accordingly depending on the server's response
            switch(serverSc.nextLine()) {
                case "WATER":
                    System.out.println("Water");
                    break;
                case "HIT":
                    System.out.println("A boat was hit!");
                    boatHit = true;
                    break;
                case "Coordinates out of bounds. Use coordinates like A1, B2...":
                    System.out.println("Coordinates out of bounds");
                    break;
                case "Already guessed this position. Try another.":
                    System.out.println("Already guessed this position, try another");
                    break;
                default:
                    System.out.println("You managed to get the server to respond something that " +
                                       "wasn't taken into account. You should be proud of yourself. " +
                                       "Or not. You decide.");
            }

        } catch (InputMismatchException e) {
            System.out.println("Invalid guess, guesses must be in the form [letter][1-2 digits] (ex: C5, A10)");
        }

        return boatHit;
    }

    public static boolean getPlayAgainInput(Scanner clientSc) {
        List<String> yesList = Arrays.asList("y", "ye", "yes", "ok", "yep", "s", "si", "sí");
        List<String> noList = Arrays.asList("n", "no", "nope");
        Boolean continuePlaying = null;

        while (continuePlaying == null) {
            System.out.printf("The game has finished! Do you want to play again? [Y/N] > ");
            String input = clientSc.nextLine().toLowerCase();
            
            if (noList.contains(input)) {
                continuePlaying = false;
            } else if (yesList.contains(input)) {
                continuePlaying = true;
            } else {
                System.out.println("Invalid input");
            }
        }

        return continuePlaying;
    }
}
