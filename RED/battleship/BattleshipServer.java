import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Random;

public class BattleshipServer {
    private static final int PORT = 12345;
    private static final int BOARD_SIZE = 10;
    private static final int[] SHIP_SIZES = {2, 3, 4, 5};

    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Server is running...");

            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("Client connected!");
                new Thread(new ClientHandler(clientSocket)).start();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static class ClientHandler implements Runnable {
        private final Socket clientSocket;
        private char[][] board; // Actual board state with ships
        private boolean[][] guessedPositions; // Track guessed positions
        private int totalHits; // Number of hit cells
        private int totalGuesses; // Number of guesses made

        public ClientHandler(Socket clientSocket) {
            this.clientSocket = clientSocket;
            initializeBoard();
        }

        @Override
        public void run() {
            try (PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
                 BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()))) {

                out.println("Welcome to Battleship! Use commands like BOARD, GUESS <coordinate>, RESET...");

                String inputLine;
                while ((inputLine = in.readLine()) != null) {
                    inputLine = inputLine.trim();
                    if (inputLine.equalsIgnoreCase("BOARD")) {
                        sendDisplayBoard(out);
                    } else if (inputLine.startsWith("GUESS")) {
                        handleGuess(inputLine.substring(6).trim(), out);
                    } else if (inputLine.equalsIgnoreCase("RESET")) {
                        initializeBoard();
                        out.println("OK");
                    } else {
                        out.println("Unknown command. Use BOARD, GUESS <coordinate>, or RESET.");
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                try {
                    clientSocket.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }

        private void initializeBoard() {
            board = new char[BOARD_SIZE][BOARD_SIZE];
            guessedPositions = new boolean[BOARD_SIZE][BOARD_SIZE];
            totalHits = 0;
            totalGuesses = 0;

            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    board[i][j] = ' '; // Unknown
                }
            }

            Random rand = new Random();
            for (int size : SHIP_SIZES) {
                placeShip(size, rand);
            }
        }

        private void placeShip(int size, Random rand) {
            boolean placed = false;
            while (!placed) {
                boolean horizontal = rand.nextBoolean();
                int row = rand.nextInt(BOARD_SIZE);
                int col = rand.nextInt(BOARD_SIZE);

                if (horizontal) {
                    if (col + size <= BOARD_SIZE && canPlaceShip(row, col, size, horizontal)) {
                        for (int i = 0; i < size; i++) {
                            board[row][col + i] = 'x';
                        }
                        placed = true;
                    }
                } else {
                    if (row + size <= BOARD_SIZE && canPlaceShip(row, col, size, horizontal)) {
                        for (int i = 0; i < size; i++) {
                            board[row + i][col] = 'x';
                        }
                        placed = true;
                    }
                }
            }
        }
/*
        private boolean canPlaceShip(int row, int col, int size, boolean horizontal) {
            for (int i = 0; i < size; i++) {
                if (horizontal) {
                    if (board[row][col + i] != ' ') return false;
                } else {
                    if (board[row + i][col] != ' ') return false;
                }
            }
            return true;
        }
*/	
	private boolean canPlaceShip(int row, int col, int size, boolean horizontal) {
    int rowStart = Math.max(0, row - 1);
    int rowEnd = Math.min(BOARD_SIZE - 1, horizontal ? row + 1 : row + size);
    int colStart = Math.max(0, col - 1);
    int colEnd = Math.min(BOARD_SIZE - 1, horizontal ? col + size : col + 1);

    for (int r = rowStart; r <= rowEnd; r++) {
        for (int c = colStart; c <= colEnd; c++) {
            if (board[r][c] != ' ') return false; // Cell is not empty, ship cannot be placed here
        }
    }
    return true;
}

	private void sendDisplayBoard(PrintWriter out) {
	    for (int i = 0; i < BOARD_SIZE; i++) {
        	for (int j = 0; j < BOARD_SIZE; j++) {
            		if (guessedPositions[i][j]) {
                		out.print(board[i][j] == 'x' ? 'x' : '~'); // 'x' for hits, '~' for water
            		} else {
               			 out.print('.'); // '.' for unknown cells
            		}
			out.print(" ");
        	}
        	out.println();
    		}		
	}

	
        private void handleGuess(String coordinate, PrintWriter out) {
            if (coordinate.length() < 2 || coordinate.length() > 3) {
                out.println("Invalid input. Use coordinates like A1, B2...");
                return;
            }

            char rowChar = coordinate.charAt(0);
            String colString = coordinate.substring(1);
            int row = rowChar - 'A';
            int col = colString.equals("10") ? 9 : Integer.parseInt(colString) - 1;

            if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
                out.println("Coordinates out of bounds. Use coordinates like A1, B2...");
                return;
            }

            if (guessedPositions[row][col]) {
                out.println("Already guessed this position. Try another.");
            } else {
                guessedPositions[row][col] = true;
                totalGuesses++;
                if (board[row][col] == 'x') {
                    totalHits++;
                    out.println("HIT");
                } else {
                    out.println("WATER");
                }
            }
        }
    }
}
