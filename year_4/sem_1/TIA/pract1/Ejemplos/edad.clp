(deftemplate edad 0 100 anos
    ((joven (10 0) (15 1) (25 1) (30 0))
    (adulta (20 0) (30 0.7) (40 1) (60 0.7) (70 0))
    (madura (60 0) (70 1))))

(defrule fuzzy1
    (edad ?f)
    =>
    (bind ?e (maximum-defuzzify ?f))
    (printout t "En fuzzy1, edad es " ?e crlf))

(defrule fuzzy2
    (edad ?f)
    =>
    (bind ?e (moment-defuzzify ?f))
    (printout t "En fuzzy2, edad es " ?e crlf))