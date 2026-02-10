'use strict'

var AnimalClass = (function () {
    function AnimalClass (name) {
        this.name = name;
    }
    AnimalClass.prototype.talk = function () { return "xx" };
    AnimalClass.prototype.animalName = function () { return this.name; };
    return AnimalClass;
}());


var PerroClass = (function (superClass) {
    function PerroClass () {
        var sup = new superClass("perro");
        for (var prop in sup) {
        	this[prop] = sup[prop]
        }
	    this ["talk"]= function () { return "guau" };
    }
    return PerroClass;
} (AnimalClass));

let animal = new AnimalClass("animalito");
console.log (animal.animalName() + " dice " + animal.talk())

let perro = new PerroClass();
console.log (perro.animalName() + " dice " + perro.talk())


