'use strict'

//
// Resulta más legible declarar la clase al completo 
//
// Aunque hemos de recordar que será posible añadir métodos/atributos
// si nos interesara.
//
let SimpleAnimalClass = function () {
	this.name = "perro"
	SimpleAnimalClass.prototype.talk = function () {return "guau"}
}

console.log (new SimpleAnimalClass().talk ())
