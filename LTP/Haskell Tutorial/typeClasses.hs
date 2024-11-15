import Data.List
import System.IO

data Employee = Employee {
    name :: String,
    position :: String,
    idNum :: Int
} deriving (Eq, Show)  -- Eq is to check for equality

samSmith = Employee {
    name = "Sam Smith",
    position = "Manager",
    idNum = 1000
}

pamMarx = Employee {
    name = "Pam Marx",
    position = "Sales",
    idNum = 1001
}

isSamPam = samSmith == pamMarx

samSmithData = show samSmith