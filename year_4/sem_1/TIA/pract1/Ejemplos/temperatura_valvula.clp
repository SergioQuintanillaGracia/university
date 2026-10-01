(deftemplate temp 5 50 celsius
    ((frio (z 10 20))
    (templado (pi 5 25))
    (calor (s 30 40))))

(deftemplate valvula 0 90 grados-apertura
    ((poco (z 10 30))
    (medio (pi 30 45))
    (mucho (s 70 80))))

(defrule hace_frio
    (temp frio)
    =>
    (assert (valvula mucho)))

(defrule temperatura_buena
    (temp templado)
    =>
    (assert (valvula medio)))

(defrule hace_calor
    (temp calor)
    =>
    (assert (valvula poco)))

(deffacts ejemplo
    (temp very templado))

(defrule defuzzificar
    (valvula ?val)
    =>
    (printout t "Apertura valvula por moment: " (moment-defuzzify ?val) crlf)
    (printout t "Apertura valvula por maximum: " (maximum-defuzzify ?val) crlf))