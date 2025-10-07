//Ejecución de varias operaciones asíncronas paralelizadas al estilo fork-join.

console.log("Inicio");
forkJoinAsync(10, () => console.log("FIN de forkJoinAsin"));

function forkJoinAsync(numeroInstancias,callback){
	
  var continuar = parseInt (numeroInstancias) || -1;
  if (continuar<0) return;// false;

  const num = numeroInstancias;
  var join = num;
  
  for(var i=0; i<num; i++) {
		let retardo = intRandom (0,10000);
    setTimeout (function(indice, ret){
			return function() {
					console.log("                   retardo:  "+ret);
					join--;
					if(join == 0) {
							console.log("soy el:  "+indice +  "  - - - - - - - - - - - - - - ->> Y SOY EL ÚLTIMO");
			    		callback();
					}
			   	else  {
			   			console.log("soy el:  "+indice + " quedan: " + join + "  - ->> y no soy el último");
			   	}
			}
		} (i, retardo),retardo);
  }
}

function intRandom(min, max) {
   return Math.floor(Math.random() * (max - min)) + min;
}
