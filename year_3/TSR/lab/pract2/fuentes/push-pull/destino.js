const {zmq, error, lineaOrdenes, traza, adios, creaPuntoConexion} = require('../tsr')
lineaOrdenes("nombre port")

var entrada = zmq.socket('pull')
creaPuntoConexion(entrada, port)

function procesaMensaje(filtro, nombre, iteracion) {
	if (!iteracion) {
		iteracion = nombre
		nombre = filtro
		filtro = ""
	}
	traza('procesaMensaje','filtro nombre iteracion',[filtro, nombre, iteracion])
}
entrada.on('message', procesaMensaje)
entrada.on('error', (msg) => {error(`${msg}`)})
process.on('SIGINT', adios([entrada],"abortado con CTRL-C"))
