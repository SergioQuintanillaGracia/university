(deftemplate edad
    0 120 anos
    ((infantil (12 1) (20 0))
    (joven (10 0) (15 1) (25 1) (30 0))
    (adulta (20 0) (30 1) (60 1) (70 0))
    (mayor (60 0) (70 1))))

(deftemplate singleton
    0 10 unit
    ((tres (3 0) (3 1) (3 0))
    (cinco (5 0) (5 1) (5 0))))

(deftemplate estatura 0 250 cm
    ((bajo (0 1) (100 1) (150 0))
    (medio (100 0) (150 1) (170 1) (180 0))
    (alto (170 0) (180 1))))

(deftemplate temperatura 30 50 grados
    ((bajo (35 1) (37 0))
    (medio (35 0) (36 1) (37 0))
    (alto (36 0) (37 1))))

(deftemplate necesidad-reasfaltado 0 100 unidades
    ((baja (z 10 25))
    (media (pi 15 60))
    (urgente (s 55 90))))

(deftemplate estatura 0 250 cm
    ((bajo (0 1) (100 1) (150 0))
    (muybajito extremely bajo)
    (medio (100 0) (150 1) (170 1) (180 0))
    (alto (170 0) (180 1))
    (muy-alto very alto)))

(deftemplate temperatura 0 80 grados
    ((frio (z 10 25))
    (congelado plus frio)))

(deftemplate temperatura 0 80 grados
    ((frio (z 10 25))
    (calor (s 30 40))
    (templado not [ frio OR calor ])))

(assert (edad adulta))
(assert (estatura very bajo))

(deffacts ejemplo1
    (edad adulta)
    (estatura very alto))

(deftemplate edad 0 100 años
    ((joven (10 0) (15 1) (25 1) (30 0))
    (adulta (20 0) (30 1) (60 1) (70 0))
    (madura (60 0) (70 1))))

(deftemplate estatura 0 250 cm
    ((bajo (0 1) (100 1) (150 0))
    (medio (100 0) (150 1) (170 1) (180 0))
    (alto (170 0) (180 1))))

(assert (edad adulta))
(assert (edad joven OR adulta OR madura))
(assert (edad joven AND adulta))
(assert (estatura very alto))

(assert (grupo pocos))
(assert (grupo (1 0) (5 1) (7 0)))
(assert (grupo NOT [ very pocos OR muchos ]))
(assert (grupo (z 4 8)))


(deffunction fuzzify (?fztemplate ?value ?delta)
    (bind ?low (get-u-from ?fztemplate))
    (bind ?hi (get-u-to ?fztemplate))
    (if (<= ?value ?low)
        then
            (assert-string
                (format nil "(%s (%g 1.0) (%g 0.0))" ?fztemplate ?low ?delta))
        else
            (if (>= ?value ?hi)
                then
                    (assert-string
                        (format nil "(%s (%g 0.0) (%g 1.0))"
                        ?fztemplate (- ?hi ?delta) ?hi))
                else
                    (assert-string
                        (format nil "(%s (%g 0.0) (%g 1.0) (%g 0.0))"
                        ?fztemplate (max ?low (- ?value ?delta))
                        ?value (min ?hi (+ ?value ?delta)) ))
)))


(fuzzify edad 35 0.1)
(fuzzify edad 35 0)

(defrule leerconsola
    (initial-fact)
    =>
    (printout t "Introduzca la edad: joven, adulta, madura" crlf)
    (bind ?Redad (read))
    (assert-string (format nil "(edad %s)" ?Redad)))

(defrule leerconsola
    (initial-fact)
    =>
    (printout t "Introduzca la edad en anos" crlf)
    (bind ?Redad (read))
    (fuzzify edad ?Redad 0.1))

(defrule leerconsola
    (initial-fact)
    =>
    (printout t "Introduzca la edad en anos" crlf)
    (bind ?Redad (read))
    (assert (edad (?Redad 0) (?Redad 1) (?Redad 0))))

(deftemplate tanque 0 80 litros
    ((bajo (10 1)(30 0))
    (medio (20 0)(35 1)(45 1)(60 0))
    (alto (50 0)(70 1))))

(assert (tanque plus alto))

(defrule danger
    (tanque extremely alto)
    =>
    (printout t "El tanque puede desbordarse. PELIGRO!" crlf)
    (assert (alarma)))

(deftemplate edad 0 100 años
    ((joven (10 0) (15 1) (25 1) (30 0))
    (veinticinco (25 0) (25 1) (25 0))          ;definimos el singleton, edad 25.
    (adulta (20 0) (30 1) (60 1) (70 0))
    (madura (60 0) (70 1))))

(deftemplate estatura 0 250 cm
    ((bajo (0 1) (100 1) (150 0))
    (medio (100 0) (150 1) (170 1) (180 0))
    (alto (170 0) (180 1))))

(deffacts ejemplo
    (edad veinticinco))

(defrule edades
    (edad adulta)
    =>
    (printout t "Los adultos son altos" crlf)
    (assert (estatura alto)))

(assert (temp frio))
(assert (temp templado))