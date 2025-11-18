const {lineaOrdenes, traza, error, adios, conecta} = require('../tsr')
zmq = require("zeromq/v5-compat")
lineaOrdenes("id brokerHost brokerPort")
let req = zmq.socket('req')
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
