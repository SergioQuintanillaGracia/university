import Data.List
import System.IO

-- Tuples can contain values of different types
randTuple = (1, "Random Tuple", False)

bobSmith = ("Bob Smith", 52)
bobsName = fst bobSmith  -- fst stands for first
bobsAge = snd bobSmith   -- snd stands for second

names = ["Bob", "Mary", "Tom"]
addresses = ["123 Main", "234 North", "567 South"]

-- We combine both into tuples with zip
namesAndAddresses = zip names addresses
