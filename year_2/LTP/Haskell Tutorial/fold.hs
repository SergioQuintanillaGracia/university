import Data.List
import System.IO

listMult = foldl (*) 1 [2, 3, 4, 5]  -- 1 * 2 * 3 * 4 * 5
listSubL = foldl (-) 1 [2, 3, 4, 5]  -- 1 - 2 - 3 - 4 - 5
listSubR = foldr (-) 1 [2, 3, 4, 5]  -- 2 - (3 - (4 - (5 - 1)))