import Data.List
import System.IO

doubleIfEven y =
    if (y `mod` 2 /= 0)  -- /= means !=
        then y
        else y * 2