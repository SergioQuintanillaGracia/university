import Data.List
import System.IO

data Shape = Circle Float Float Float | Rectangle Float Float Float Float
    deriving Show

area :: Shape -> Float
area (Circle _ _ r) = pi * r^2
-- `$` means that everything after it has preference, it is useful to
-- "remove" parentheses
area (Rectangle x y x2 y2) = (abs $ x2 - x) * (abs $ y2 - y)

areaCircleEx = area (Circle 50 60 20)
areaRectEx = area $ Rectangle 10 10 100 100