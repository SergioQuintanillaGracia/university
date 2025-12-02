const {zmq, lineaOrdenes, traza, error, adios, conecta} = require('../tsr')
lineaOrdenes("id num_peticiones brokerHost brokerPort")

let req = zmq.socket('req')

// Connection ID as specified as argument. If we specify "NH", id will be created based on own hostname
id = (id=="HN") ? "CLI_"+require('os').hostname() : id 
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
