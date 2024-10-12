## SUMMARY OF HOW THE CLIENT WORKS
My Battleship game client works as follows:

- The client connects to the server using a socket.
- A `PrintWriter` and two `Scanners` are created, which are used to send requests to the server, and get the user's input and the server's responses.
- A main loop starts, which resets the board and starts new games. It continues running until the user decides not to continue playing.
- The game board is printed and the user is asked to make a guess. The input is validated and sent to the server, whose response is used to determine if a boat was hit or not.
- The game board is updated. The user continues being asked to make a guess until 14 hits are made, which means that all boats have been sunk and the game ended.

## DESIGN CHOICES
- When creating the `Socket`, `PrintWriter`, and `Scanners`, `UnknownHostException`, `NoRouteToHostException` and `IOException` exceptions are handled. All of them are closed in the `finally` block.
- I coded several functions to make the main `while` loop more readable, concise, and easier to understand:
    - `printCount()`: Prints a specific number of lines read from the server socket.
    - `printBoard()`: Requests and prints the game board. It prints a number row at the top, and a letter column at the left, so it's easier for the user to know the coordinates of the spot they want to guess.
    - `getGuess()`: Asks the user for a coordinate, validates the input, and returns it. It throws `InputMismatchException` if the input is invalid, which is handled in `handleGuess()`.
    - `handleGuess()`: Handles the different possible server outputs to a guess.
    - `getPlayAgainInput()`: Asks the user whether they want to play another game. It checks if their answer is in arrays containing words for "yes" or "no" to determine their decision.