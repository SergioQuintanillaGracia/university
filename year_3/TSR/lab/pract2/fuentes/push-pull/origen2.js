const {zmq, lineaOrdenes, error, adios, conecta} = require('../tsr')
lineaOrdenes("nombre hostSig1 portSig1 hostSig2 portSig2")

let salida = zmq.socket('push')
conecta(salida, hostSig1, portSig1)
conecta(salida, hostSig2, portSig2)

salida.on('error', (msg) => {error(`${msg}`)})
process.on('SIGINT', adios([salida],"abortado con CTRL-C"))

for (let i=1; i<=4; i++) {
	console.log(`enviando mensaje: [${nombre},${i}]`)
	salida.send([nombre,i])
}
