import Data.ByteString.Lazy.Char8 (replicate)
type Side = Float
type Apothem = Float
type Radius = Float

data Pentagon = Pentagon Side Apothem
data Circle = Circle Radius

class Shape a where
    perimeter :: a -> Float
    area :: a -> Float

instance Shape Pentagon where
    perimeter (Pentagon s a) = 5 * s
    area (Pentagon s a) = (5 * s * a) / 2

instance Shape Circle where
    perimeter (Circle r) = 2 * pi * r
    area (Circle r) = pi * r * r