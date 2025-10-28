const {zmq, lineaOrdenes, traza, error, adios, conecta} = require('../tsr')
lineaOrdenes("identidad ipPublicador portPublicador tema")

let sub = zmq.socket('sub')
sub.subscribe(tema)
conecta(sub, ipPublicador, portPublicador)

function recibeMensaje(tema, numero, ronda) {
	traza('recibeMensaje','tema numero ronda',[tema, numero, ronda])
}

    sub.on('message', recibeMensaje)
    sub.on('error'  , (msg) => {error(`${msg}`)})
process.on('SIGINT' , adios([sub],"abortado con CTRL-C"))
