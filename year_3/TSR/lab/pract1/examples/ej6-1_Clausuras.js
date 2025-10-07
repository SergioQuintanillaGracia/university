'use strict'

function f1() {
	console.log ("hola")
}
f1();

let x = "hola"
function f2() {
	console.log (x)
}
f2();


function f3 (arg) {
	let i=0;
	return function () {
		i++;
		console.log (arg + i);
	} 
}

let f = f3 ("hola");
f();
f();
