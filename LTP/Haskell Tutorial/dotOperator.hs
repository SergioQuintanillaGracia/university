import Data.List
import System.IO

sumValue = putStrLn (show (1 + 2))

-- The dot allows us to send the output of `show` to
-- `putStrLn`, it's used for chaining functions
sumValue2 = putStrLn . show $ 1 + 2