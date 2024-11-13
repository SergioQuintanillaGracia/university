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