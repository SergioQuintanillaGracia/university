import Data.List
import System.IO

sumOfNums = sum [1..1000]  -- Sums numbers from 1 to 1000
addEx = 5 + 4
multEx = 5 * 4
modEx = mod 5 4
modEx2 = 5 `mod` 4  -- Infix operator

negNumEx = 5 + (-4)

num9 = 9 :: Int  -- num9 is an Int with the value 9
sqrtOf9 = sqrt (fromIntegral num9)  -- fromIntegral converts Int to Float

piEx = pi
expEx = exp 9  -- Calculates e^x (e^9 in this case)
logEx = log 2.71
logEx2 = log (exp 1)
squared9 = 9 ** 2
truncateEx = truncate 9.9
roundEx = round 9.9
ceilingEx = ceiling 9.9
floorEx = floor 9.9

-- There are also:
-- sin, cos, tan, asin, acos, atan

trueAndFalse = True && False
trueOrFalse = True || False
notTrue = not True