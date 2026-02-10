//Uso de la sentencia let.

var i = 0;

do {
	var k = i;
	setTimeout(function() {console.log(k)}, k * 1000);
	i++;    
} while (i<10);

console.log("Terminado codigo, valor de i: " + i);
