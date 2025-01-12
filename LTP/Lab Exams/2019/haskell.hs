member :: (Eq a) => a -> [a] -> Bool
member n [] = False
member n (x:xs) = (n == x) || member n xs

union :: (Eq a) => [a] -> [a] -> [a]
union [] ys = ys
union (x:xs) ys
    | member x ys = union xs ys
    | otherwise = x : union xs ys


data BinTreeInt = Void | Node Int BinTreeInt BinTreeInt deriving Show
sizeTree :: BinTreeInt -> Int
sizeTree Void = 0
sizeTree (Node a left right) = 1 + sizeTree left + sizeTree right