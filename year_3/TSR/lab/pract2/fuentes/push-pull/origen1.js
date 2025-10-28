const {zmq, lineaOrdenes, error, adios, conecta} = require('../tsr')
lineaOrdenes("nombre hostSig portSig")

let salida = zmq.socket('push')
conecta(salida, hostSig, portSig)

salida.on('error', (msg) => {error(`${msg}`)})
process.on('SIGINT', adios([salida],"abortado con CTRL-C"))

for (let i=1; i<=4; i++) {
	console.log(`enviando mensaje: [${nombre},${i}]`)
	salida.send([nombre,i])
}
