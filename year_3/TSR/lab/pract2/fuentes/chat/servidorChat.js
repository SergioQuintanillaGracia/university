const {zmq, lineaOrdenes, error, adios, creaPuntoConexion} = require('../tsr')
lineaOrdenes("portDifusion portEntrada")

let entrada = zmq.socket('pull')
let salida  = zmq.socket('pub')
creaPuntoConexion(salida, portDifusion)
creaPuntoConexion(entrada,portEntrada)

entrada.on('message', (id,txt) => {
	switch (txt.toString()) {
		case 'HI':  salida.send(['server', id+' connected']); break
		case 'BYE': salida.send(['server', id+' disconnected']); break
		default:    salida.send([id,txt])
	}
})

salida.on('error', (msg) => {error(`${msg}`)})
process.on('SIGINT', adios([entrada,salida],"abortado con CTRL-C"))
