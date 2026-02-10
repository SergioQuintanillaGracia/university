const {zmq, lineaOrdenes, traza, error, adios, conecta} = require('../tsr')
lineaOrdenes("id brokerHost brokerPort")
let req = zmq.socket('req')
req.identity = id
conecta(req, brokerHost, brokerPort)
req.send(['','',''])

function procesaPeticion(cliente, separador, mensaje) {
	traza('procesaPeticion','cliente separador mensaje',[cliente, separador, mensaje])
	setTimeout(()=>{req.send([cliente,'',`${mensaje} ${id}`])}, 1000)
}
req.on('message', procesaPeticion)
req.on('error', (msg) => {error(`${msg}`)})
process.on('SIGINT', adios([req],"abortado con CTRL-C"))