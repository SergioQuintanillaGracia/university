import Data.List
import System.IO

getListItems :: [Int] -> String
getListItems [] = "Your list is empty"
getListItems (x:[]) = "Your list starts with " ++ show x
getListItems (x:y:[]) = "Y0ur list contains " ++ show x ++ " and " ++ show y
getListItems(x:xs) = "The 1st item is " ++ show x ++ " and the rest are " ++ show xs