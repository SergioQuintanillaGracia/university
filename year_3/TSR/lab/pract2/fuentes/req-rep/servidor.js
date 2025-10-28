const {zmq, error, lineaOrdenes, adios, traza, creaPuntoConexion} = require('../tsr')
lineaOrdenes("identidad port segundos saludo")

var rep = zmq.socket('rep')
creaPuntoConexion(rep, port)

function procesaPeticion(nombre, iteracion) {
	traza('procesaPeticion','nombre iteracion',[nombre,iteracion])
	setTimeout(()=>{rep.send([identidad, saludo, nombre, iteracion])}, parseInt(segundos)*1000) //calcula y envía respuesta
}

rep.on('message', procesaPeticion)
rep.on('error', (msg) => {error(`${msg}`)})
process.on('SIGINT', adios([rep],"abortado con CTRL-C"))