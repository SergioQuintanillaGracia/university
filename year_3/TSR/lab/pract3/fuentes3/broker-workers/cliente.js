const {lineaOrdenes, traza, error, adios, conecta} = require('../tsr')
zmq = require("zeromq/v5-compat")
lineaOrdenes("id num_peticiones brokerHost brokerPort")

let req = zmq.socket('req')
req.identity = id
conecta(req, brokerHost, brokerPort)

let count = 1; // Requests counter
req.send (id + "-" + count)

function procesaRespuesta(msg) {
	traza('procesaRespuesta','msg',[msg])
    if (++count > num_peticiones) {
    	adios([req], `Recibido: ${msg}. Adios`)()
    } else {
        req.send (id + "-" + count)
    }
}

req.on('message', procesaRespuesta)
req.on('error', (msg) => {error(`${msg}`)})
process.on('SIGINT', adios([req],"abortado con CTRL-C"))
