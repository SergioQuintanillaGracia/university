const {zmq, lineaOrdenes, traza, error, adios, conecta} = require('../tsr')
lineaOrdenes("id brokerHost brokerPort")
let rep = zmq.socket('rep')
conecta(rep, brokerHost, brokerPort)

function procesaPeticion(mensaje) {
	traza('procesaPeticion','mensaje',[mensaje])
	setTimeout(()=>{rep.send(`${mensaje} ${id}`)}, 1000)
}

    rep.on('message', procesaPeticion)
    rep.on('error'  , (msg) => {error(`${msg}`)})
process.on('SIGINT' , adios([rep],"abortado con CTRL-C"))
