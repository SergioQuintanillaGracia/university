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


-- EX 12
data Tree a = Leaf a | Branch (Tree a) (Tree a) deriving Show

symmetric :: Tree a -> Tree a
symmetric (Leaf a) = Leaf a
symmetric (Branch a b) = Branch (symmetric b) (symmetric a)


-- EX 13
listToTree :: [a] -> Tree a
listToTree [x] = Leaf x
listToTree xs = Branch (listToTree(take size xs)) (listToTree(drop size xs))
    where size = length xs `div` 2

treeToList :: Tree a -> [a]
treeToList (Leaf x) = [x]
treeToList (Branch a b) = treeToList a ++ treeToList b


-- EX 14
data BinTreeInt = Void | Node Int BinTreeInt BinTreeInt deriving Show

insTree :: Int -> BinTreeInt -> BinTreeInt
insTree x Void = Node x Void Void
insTree x (Node y a b)
    | x < y = Node y (insTree x a) b
    | otherwise = Node y a (insTree x b)


-- EX 15
creaTree :: [Int] -> BinTreeInt
creaTree [] = Void
creaTree (x:xs) = insTree x (creaTree xs)


-- EX 16
treeElem :: Int -> BinTreeInt -> Bool
treeElem x Void = False
treeElem x (Node a left right)
    | x < a = treeElem x left
    | x > a = treeElem x right
    | x == a = True


-- EX 17
repeated :: Int -> [Int] -> Int
repeated n [] = 0
repeated n (x:xs)
    | n == x = 1 + repeated n xs
    | otherwise = repeated n xs


-- EX 18
concat' :: [[a]] -> [a]
concat' [] = []
concat' (x:xs) = x ++ concat' xs


-- EX 19
square :: Int -> Int
square x = x * x
res = (sum . map square . filter even) [1..10]
-- Returns the addition of the squares of the even numbers from 1 to 10


-- EX 20
-- data Tree a = Leaf a | Branch (Tree a) (Tree a) deriving Show

numLeaves :: Tree a -> Int
numLeaves (Leaf a) = 1
numLeaves (Branch left right) = numLeaves left + numLeaves right