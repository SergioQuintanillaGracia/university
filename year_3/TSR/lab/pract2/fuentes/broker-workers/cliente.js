const {zmq, lineaOrdenes, traza, error, adios, conecta} = require('../tsr')
lineaOrdenes("id brokerHost brokerPort")
let req = zmq.socket('req')
req.identity = id
conecta(req, brokerHost, brokerPort)
req.send(id)

function procesaRespuesta(msg) {
	traza('procesaRespuesta','msg',[msg])
	adios([req], `Recibido: ${msg}. Adios`)()
}
req.on('message', procesaRespuesta)
req.on('error', (msg) => {error(`${msg}`)})
process.on('SIGINT', adios([req],"abortado con CTRL-C"))
