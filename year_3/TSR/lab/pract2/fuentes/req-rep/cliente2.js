const {zmq, lineaOrdenes, traza, error, adios, conecta} = require('../tsr')
lineaOrdenes("ipServidor1 portServidor1 ipServidor2 portServidor2 nombre")

let req = zmq.socket('req')
conecta(req, ipServidor1, portServidor1)
conecta(req, ipServidor2, portServidor2)

function procesaRespuesta(idServidor, saludo, nombre, iteracion) {
	traza('procesaRespuesta', 'idServidor saludo nombre iteracion', [idServidor,saludo,nombre,iteracion])
	if (iteracion==4) adios([req], "He terminado")()
}

req.on('message', procesaRespuesta)
req.on('error', (msg) => {error(`${msg}`)})
process.on('SIGINT', adios([req],"abortado con CTRL-C"))

for (let i=1; i<=4; i++) {
	console.log(`enviando mensaje: [${nombre},${i}]`)
	req.send([nombre,i])
}
