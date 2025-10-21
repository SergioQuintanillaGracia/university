module ExamplesP4 where
  import Data.Char

  -- Solved exercise 1
  nextchar :: Char -> Char
  nextchar c = chr (ord c + 1)

  -- Solved exercise 2
  fact :: Int -> Int
  fact 0 = 1
  fact x = x * fact (x - 1)

  -- Exercise 1
  numCbetw2 :: Char -> Char -> Int
  numCbetw2 x y
    | x /= y = abs (ord y - ord x) - 1
    | otherwise = 0

  -- Exercise 2
  addRange :: Int -> Int -> Int
  addRange x y
    | x == y = x
    | x < y = x + addRange (x + 1) y
    | y < x = y + addRange (y + 1) x

  -- Exercise 3
  max' :: Int -> Int -> Int
  max' x y
    | x > y = x
    | otherwise = y
  
  -- Exercise 4
  leapyear :: Int -> Bool
  leapyear x
    | mod x 100 == 0 && mod x 400 == 0 = True
    | mod x 100 == 0 = False
    | mod x 4 == 0 = False
    | otherwise = False

  -- Exercise 5
  daysAmonth :: Int -> Int -> Int
  daysAmonth x y
    | x == 1 || x == 3 || x == 5 || x == 7 || x == 8 || x == 10 || x == 12 = 31
    | x == 4 || x == 6 || x == 9 || x == 11 = 30
    | x == 2 = if leapyear y then 29 else 28
    | otherwise = -1
  
  -- Exercise 6
  remainder :: Int -> Int -> Int
  remainder x y
    | x >= y = remainder (x - y) y
    | otherwise = x
  
  -- Exercise 7
  sumFacts :: Int -> Int
  sumFacts 0 = 0
  sumFacts x = fact x + sumFacts (x - 1)



  signum' x = if x < 0 then -1 else
              if x == 0 then 0 else 1

  signum'' x 
           | x < 0     = -1 
           | x == 0    = 0 
           | otherwise = 1 

  
  -- right hello func:
  hello n = concat (replicate n "hello ")
  hello' n = putStrLn (concat (replicate n "hello \n"))
  

  convert :: (Char, Int) -> String
  convert (c,i) = [c] ++ show i

  -- main = convert (2,'c')
  main = convert ('c',2)


  length' [] = 0
  length' (x:t) = 1 + length' t


  add :: (Int , Int) -> Int
  add (x,y) = x + y

  cAdd :: Int -> Int -> Int
  cAdd x y = x + y


  power1 :: Int -> Int -> Int
  power1 _ 0 = 1
  power1 n t = n * power1 n (t - 1)

  power2 :: Int -> Int -> Int
  power2 _ 0 = 1
  power2 n t
         | even t = power2 (n * n) (div t 2)
         | otherwise = n * power2 (n * n) (div t 2)


  squarepow :: Int -> Int
  squarepow x = x * x

  doubleHO :: (Int -> Int) -> Int -> Int
  doubleHO f x = f (f x)

  fourthpow :: Int -> Int
  fourthpow = doubleHO squarepow
