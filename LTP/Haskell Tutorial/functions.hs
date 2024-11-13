import Data.List
import System.IO

addMe :: Int -> Int -> Int

-- Function structure: funcName param1 param2 = operations (returned value)
addMe x y = x + y

-- As we didn't define we are only working with integers, we can use
-- floating point values as well
sumMe x y = x + y

addTuples :: (Int, Int) -> (Int, Int) -> (Int, Int)
addTuples (x, y) (x2, y2) = (x + x2, y + y2)

whatAge :: Int -> String
whatAge 16 = "You can drive"
whatAge 18 = "You can vote"
whatAge 21 = "You are an adult"
whatAge _ = "Nothing important"  -- We could also put something like `x` instead of _