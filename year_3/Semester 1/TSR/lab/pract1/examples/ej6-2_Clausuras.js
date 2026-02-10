
// Primer segmento de código. Variable global.
var x;
for (x=11; x<14; x++) {
	console.log (x)
}

// Segundo segmento de código. Función generadora.
function gen () { 
	var x;
	return function () {
		for (x=21; x<24; x++) {
			console.log (x)
		}
	}
}
let f = gen();
f();

// Tercer segmento de código. Función generadora
// anónima.
( function () { 
	var x;
	return function () {
		for (x=31; x<34; x++) {
			console.log (x)
		}
	}
}()() )
