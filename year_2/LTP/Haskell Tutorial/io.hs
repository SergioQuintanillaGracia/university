import Data.List
import System.IO

sayHello = do
    putStrLn "Enter name"
    name <- getLine
    putStrLn $ "Hello " ++ name

writeToFile = do
    file <- openFile "test.txt" WriteMode
    hPutStrLn file ("Random line of text")
    hClose file

readFromFile = do
    file <- openFile "test.txt" ReadMode
    contents <- hGetContents file
    putStr contents
    hClose file