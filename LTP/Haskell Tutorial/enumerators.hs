import Data.List
import System.IO

data BaseballPlayer = Pitcher
                    | Catcher
                    | Infielder
                    | Outfielder
                deriving Show

barryBonds :: BaseballPlayer -> Bool
barryBonds Outfielder = True


data Customer = Customer String String Double
    deriving Show

tomSmith :: Customer
tomSmith = Customer "Tom Smith" "123 Main" 20.50

getBalance :: Customer -> Double
getBalance (Customer _ _ b) = b


data RPS = Rock | Paper | Scissors

shoot :: RPS -> RPS -> String
shoot Paper Rock = "Paper beats rock"
-- ...
shoot _ _ = "Unhandled case"