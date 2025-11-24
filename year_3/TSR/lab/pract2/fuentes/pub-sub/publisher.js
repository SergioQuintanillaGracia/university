const {error, lineaOrdenes, traza, adios, creaPuntoConexion} = require('../tsr')
zmq = require("zeromq/v5-compat")
port = parseInt(process.argv[2])
numMessages = parseInt(process.argv[3])
let temas = process.argv.slice(4)

var pub = zmq.socket('pub')
creaPuntoConexion(pub, port)

function envia(tema, numMensaje, ronda) {
	console.log(tema, numMensaje, ronda)
	pub.send([tema, numMensaje, ronda]) 
}
function publica(i) {
	return () => {
		envia(temas[i%3], i + 1, Math.trunc(i/3 + 1))
		if (i==numMessages - 1) adios([pub],"No me queda nada que publicar. Adios")()
		else setTimeout(publica(i+1),1000)
	}
}
setTimeout(publica(0), 1000)

pub.on('error', (msg) => {error(`${msg}`)})
process.on('SIGINT', adios([pub],"abortado con CTRL-C"))