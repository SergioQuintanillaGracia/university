module Geometry2D (areaSquare, perimeterSquare) where
    areaRectangle :: Float -> Float -> Float
    areaRectangle b h = b * h

    perimeterRectangle :: Float -> Float -> Float
    perimeterRectangle b h = 2 * (b + h)

    areaSquare :: Float -> Float
    areaSquare s = areaRectangle s s

    perimeterSquare :: Float -> Float
    perimeterSquare s = perimeterRectangle s s
