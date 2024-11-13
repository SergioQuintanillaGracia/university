import Data.List
import System.IO

isOdd :: Int -> Bool
isOdd n
    | n `mod` 2 == 0 = False
    | otherwise = True

whatGrade :: Int -> String
whatGrade age
    | (age >= 5) && (age <= 6) = "Kindergarten"
    | (age > 6) && (age <= 10) = "Elementary school"
    | otherwise = "Not kindergarten nor elementary school"