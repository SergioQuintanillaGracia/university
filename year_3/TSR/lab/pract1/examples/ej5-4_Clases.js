'use strict';

//
// Podemos crear objetos rápidamente, sin más que detallar el valor de
// los atributos. 
//
// En este caso no estamos creando una clase, sino directamente
// una única instancia.
//

var animal = {
	name: "perro",
	talk: function () {return "guau"}
}

console.log (animal.talk ())
