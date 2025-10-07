'use strict'

// Una simple función se comporta como una clase si usamos "new"
let SimpleAnimalClass = function () {
	this.name = "perro"
}

// Observa que usamos "new"
let animal = new SimpleAnimalClass();

// Podemos consultar atributos
console.log (animal.name)

// Podemos añadir atributos
animal.patas = 4;
console.log (animal.patas)

// Podemos añadir métodos a una clase en tiempo de ejecución.
SimpleAnimalClass.prototype.talk = function () {return "guau"}
animal.bark = () => {return "guau"}

console.log (animal.talk ())
console.log (animal.bark ())

