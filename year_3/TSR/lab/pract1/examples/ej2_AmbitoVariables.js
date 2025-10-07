'use strict';

let global1_let = "variable global 1"
var global2_var = "variable global 2"
global3 = "variable global 3"

function f1(arg) {
	global1_let = arg;    // Puedo también modificar las otras globales?
	global2_var = arg;    // YES
	global3 = arg;		  // YES
	console.log("global1= " + global1_let)
	console.log("global2= " + global2_var)
	console.log("global3= " + global3)
}

function f2() {
	let local1_let = 5;

	for (let let_i=0; let_i<5; let_i++) {
		console.log("let_i: " + let_i)
	}

	for (var var_i=0; var_i<5; var_i++) {
		console.log("var_i: " + var_i)
	}

	console.log("fin --> var_i = " + var_i)

	local_let = var_i - 5; // Me equivoco al teclear "local_let1" 💀
	console.log("local1_let = " + local1_let)
	console.log("local_let = " + local_let)
}

f1("nuevo valor")
f2();