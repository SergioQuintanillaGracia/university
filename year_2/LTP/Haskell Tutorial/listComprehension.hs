import Data.List
import System.IO

-- List formed by 3^n, where n comes from the list [1..10]
pow3List = [3^n | n <- [1..10]]

multTable = [[x * y | x <- [1..10]] | y <- [1..10]]