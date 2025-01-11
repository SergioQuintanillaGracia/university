-- Exercise 1
decBin :: Int -> [Int]
decBin 0 = [0]
decBin 1 = [1]
decBin x = mod x 2 : decBin (div x 2)

-- Exercise 2
binDec :: [Int] -> Int
binDec [x] = x
binDec (x:xs) = x + binDec xs * 2

-- Exercise 3
divisors :: Int -> [Int]
divisors x = [n | n <- [1..x], mod x n == 0]

-- Exercise 4
member :: Int -> [Int] -> Bool
member _ [] = False
member n (x:xs)
    | n == x = True
    | otherwise = member n xs

-- Exercise 5
isPrime :: Int -> Bool
isPrime x = length (divisors x) == 2

primes :: Int -> [Int]
primes n = [x | x <- [1..n], isPrime x]

-- Exercise 6
selectEven :: [Int] -> [Int]
selectEven [] = []
selectEven (x:xs)
    | even x = x : selectEven xs
    | otherwise = selectEven xs

-- Exercise 7
selectEvenPos :: [Int] -> [Int]
selectEvenPos [x] = [x]
selectEvenPos (x:y:xs) = x : selectEvenPos xs

selectEvenPos2 :: [Int] -> [Int]
selectEvenPos2 l = [l !! n | n <- [0,2..length l - 1]]

-- Exercise 8
ins :: Int -> [Int] -> [Int]
ins n [] = [n]
ins n (x:xs)
    | n <= x = n : x : xs
    | otherwise = x : ins n xs

iSort :: [Int] -> [Int]
iSort [] = []
iSort (x:xs) = ins x (iSort xs)

-- Exercise 9
doubleAll :: [Int] -> [Int]
doubleAll xs = map (2*) xs  -- or doubleAll = map (2*)

-- Exercise 10
map' :: (a -> b) -> [a] -> [b]
map' f xs = [f x | x <- xs]

filter' :: (a -> Bool) -> [a] -> [a]
filter' f xs = [x | x <- xs, f x]


-- Definitions
type Person = String
type Book = String
type Database = [(Person, Book)]

exampleBase :: Database
exampleBase =
    [("Alicia","El nombre de la rosa"),
    ("Juan","La hija del canibal"),
    ("Pepe","Odesa"),
    ("Alicia","La ciudad de las bestias")]

obtain :: Database -> Person -> [Book]
obtain db p = [y | (x, y) <- db, x == p]

-- Exercise 11
borrow :: Database -> Book -> Person -> Database
borrow db b p = (p, b) : db

return' :: Database -> (Person, Book) -> Database
return' db (p, b) = [(p2, b2) | (p2, b2) <- db, (p, b) /= (p2, b2)]


-- Definitions
data Tree a = Leaf a | Branch (Tree a) (Tree a) deriving Show

-- Exercise 12
symmetric :: Tree a -> Tree a
symmetric (Leaf a) = Leaf a
symmetric (Branch a b) = Branch (symmetric b) (symmetric a)

-- Exercise 13
listToTree :: [a] -> Tree a
listToTree [x] = Leaf x
listToTree xs = Branch (listToTree (take size xs)) (listToTree (take size xs))
    where size = div (length xs) 2

treeToList :: Tree a -> [a]
treeToList (Leaf a) = [a]
treeToList (Branch t1 t2) = treeToList t1 ++ treeToList t2

-- Definitions
data BinTreeInt = Void | Node Int BinTreeInt BinTreeInt deriving Show

-- Exercise 14
insTree :: Int -> BinTreeInt -> BinTreeInt
insTree x Void = Node x Void Void
insTree x (Node n t1 t2)
    | x < n = Node n (insTree x t1) t2
    | otherwise = Node n t1 (insTree x t2)

-- Exercise 15
creaTree :: [Int] -> BinTreeInt
creaTree [] = Void
creaTree (x:xs) = insTree x (creaTree xs)

-- Exercise 16
treeElem :: Int -> BinTreeInt -> Bool
treeElem x Void = False
treeElem x (Node n a b)
    | x < n = treeElem x a
    | x > n = treeElem x b
    | otherwise = True