const {zmq, lineaOrdenes, traza, error, adios, conecta} = require('../tsr')
lineaOrdenes("id brokerHost brokerPort")

let req = zmq.socket('req')

// Connection ID as specified as argument. If we specify "NH", id will be created based on own hostname
id = (id=="HN") ? "WOR_"+require('os').hostname() : id 
req.identity = id

conecta(req, brokerHost, brokerPort)
req.send(['','']) // Worker registers: No client, no reply message

function procesaPeticion(cliente, mensaje) {
	traza('procesaPeticion','cliente mensaje',[cliente, mensaje])
	setTimeout(()=>{req.send([cliente,`${mensaje} ${id}`])}, 1000)
}
req.on('message', procesaPeticion)
req.on('error', (msg) => {error(`${msg}`)})
process.on('SIGINT', adios([req],"abortado con CTRL-C"))