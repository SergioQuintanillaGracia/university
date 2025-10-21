import Data.List
import System.IO

-- The part after the comma is the filter for the items
listTimes3 = [x * 3 | x <- [1..10], x * 3 <= 50]

divisibleBy9And13 = [x | x <- [1..500], x `mod` 9 == 0, x `mod` 13 == 0]

sortedList = sort [9, 1, 2, 8, 5, 9]

sumOfLists = zipWith (+) [1, 2, 3, 4, 5] [6, 7, 8, 9, 10]

list1 = [1, 2, 3, 4, 5, 6, 7, 8, 9]

listBiggerThan5 = filter (>5) list1

evensUpTo20 = takeWhile (<= 20) [2, 4..]