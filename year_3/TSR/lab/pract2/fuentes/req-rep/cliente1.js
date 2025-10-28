const {zmq, lineaOrdenes, traza, error, adios, conecta} = require('../tsr')
lineaOrdenes("ipServidor portServidor nombre")

let req = zmq.socket('req')
conecta(req, ipServidor, portServidor)

function procesaRespuesta(idServidor, saludo, nombre, iteracion) {
	traza('procesaRespuesta','idServidor saludo nombre iteracion',[idServidor, saludo, nombre, iteracion]) //traza
	if (iteracion==4) adios([req], "He terminado")()
}
req.on('message', procesaRespuesta)
req.on('error', (msg) => {error(`${msg}`)})
process.on('SIGINT', adios([req],"abortado con CTRL-C"))

for (let i=1; i<=4; i++) {
	console.log(`enviando mensaje: [${nombre},${i}]`)
	req.send([nombre,i])
}
