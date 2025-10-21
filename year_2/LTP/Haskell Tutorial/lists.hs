import Data.List
import System.IO

primeNumbers = [3, 5, 7, 11]
multList = [[1, 2, 3], [4, 5]]
morePrimes = primeNumbers ++ [13, 17, 19, 23, 29]  -- Concatenate lists
favNums = 2 : 7 : 21 : 66 : []  -- List constructor ([] marks the end of the list)
morePrimes2 = 2 : morePrimes

lenPrime = length morePrimes2
revPrime = reverse morePrimes2
isListEmpty = null morePrimes2

secondPrime = morePrimes2 !! 1  -- Get element at index 1
firstPrime = head morePrimes2
lastPrime = last morePrimes2
initPrime = init morePrimes2  -- Returns the original list minus the last element
first3Primes = take 3 morePrimes2  -- Takes the first 3 elements
removedPrimes = drop 3 morePrimes2  -- Returns the list without the first 3 elements

is7InList = 7 `elem` morePrimes2
maxPrime = maximum morePrimes2
minPrime = minimum morePrimes2

newList = [2, 3, 4]
prodNewList = product newList  -- Multiplies every element in the list

zeroToTen = [0..10]
evenList = [2,4..20]  -- The first 2 numbers set the "step" (4 - 2 = 2)
letterList = ['A', 'C'..'Z']

infinPow10 = [10, 20..]  -- Infinite list, but the values are generated just when
                         -- we need them
infin50thPos = infinPow10 !! 50

many2s = take 10 (repeat 2)  -- Take the first 10 elements of the list `repeat 2`
                             -- `repeat 2` is an infinite list of 2s

many3s = replicate 10 3  -- Replicates 3 10 times

cycleList = take 12 (cycle [1, 2, 3, 4, 5])  -- 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2