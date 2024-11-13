-- EX 1
-- (Starting from least significant bit)
decBin :: Int -> [Int]
decBin x =
    if x < 2
        then [x]
        else (x `mod` 2) : decBin(x `div` 2)


-- EX 2
-- (Starting from least significant bit)
binDec :: [Int] -> Int
binDec (x:[]) = x
binDec (x:y) = x + binDec y * 2


-- EX 3
divisors :: Int -> [Int]
divisors x = [d | d <- [1..x], x `mod` d == 0]


-- EX 4
member :: Int -> [Int] -> Bool
member n [] = False
member n (x:xs) = n == x || member n xs


-- EX 5
isPrime :: Int -> Bool
isPrime x = length (divisors x) == 2

primes :: Int -> [Int]
primes x = take x [p | p <- [1..], isPrime p]


-- EX 6
selectEven :: [Int] -> [Int]
selectEven l = [n | n <- l, n `mod` 2 == 0]


-- EX 7
selectEvenPos :: [Int] -> [Int]
selectEvenPos l = [l !! n | n <- [0,2..length l - 1]]


-- EX 8
ins :: Int -> [Int] -> [Int]
ins n [] = [n]
ins n (x:xs)
    | n <= x = n : x : xs
    | otherwise = x : ins n xs

iSort :: [Int] -> [Int]
iSort [] = []
iSort (x:xs) = ins x (iSort xs)


-- EX 9
doubleAll :: [Int] -> [Int]
doubleAll l = map (\x -> x * 2) l


-- EX 10
map' :: (a -> b) -> [a] -> [b]
map' f l = [f x | x <- l]

filter' :: (a -> Bool) -> [a] -> [a]
filter' f l = [x | x <- l, f x]


-- EX 11
type Person = String
type Book = String
type Database = [(Person, Book)]

exampleBase =
    [("Alicia","El nombre de la rosa"),
    ("Juan","La hija del canibal"),
    ("Pepe","Odesa"),
    ("Alicia","La ciudad de las bestias")]

obtain :: Database -> Person -> [Book]
obtain dBase pers = [b | (p, b) <- dBase, p == pers]

borrow :: Database -> Book -> Person -> Database
borrow db b p = (p, b) : db

return' :: Database -> (Person, Book) -> Database
return' db (p,b) = [(pers,book) | (pers,book) <- db, (p,b) /= (pers,book)]