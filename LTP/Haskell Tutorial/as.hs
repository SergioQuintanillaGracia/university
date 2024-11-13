import Data.List
import System.IO

getFirstItem :: String -> String
getFirstItem [] = "Empty string"
getFirstItem all@(x:xs) = "The first letter in " ++ all ++ " is " ++ show x