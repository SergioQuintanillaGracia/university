import Data.List
import System.IO

getClass :: Int -> String
getClass n = case n of
    5 -> "Kindergarten"
    6 -> "Elementary"
    _ -> "Go away"