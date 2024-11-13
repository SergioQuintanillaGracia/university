import Data.List
import System.IO

batAvgRating :: Double -> Double -> String
batAvgRating hits atBats
    | avg <= 0.2 = "Terrible batting average"
    | avg <= 0.25 = "Average player"
    | avg <= 0.28 = "You are doing pretty good"
    | otherwise = "You are so good"
    where avg = hits / atBats