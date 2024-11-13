import Data.List
import System.IO

getAddFunc :: Int -> (Int -> Int)
-- `getAddFunc` takes an Int x and returns another function that
-- also takes an Int y, which is why we need to put y before `=`
getAddFunc x y = x + y

adds3 = getAddFunc 3

fourPlus3 = adds3 4

threePlusList = map adds3 [1,2,3,4,5]