import Data.List
import System.IO

class MyEq a where
    areEqual :: a -> a -> Bool

data ShirtSize = S | M | L

instance MyEq ShirtSize where
    areEqual S S = True
    areEqual M M = True
    areEqual L L = True
    areEqual _ _ = False

isSizeEqual = areEqual M M