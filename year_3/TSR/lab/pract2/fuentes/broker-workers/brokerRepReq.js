const {zmq, lineaOrdenes, traza, error, adios, creaPuntoConexion} = require('../tsr')
lineaOrdenes("frontendPort backendPort")
let frontend = zmq.socket('rep')
let backend  = zmq.socket('req')
creaPuntoConexion(frontend, frontendPort)
creaPuntoConexion(backend,  backendPort)

function procesaPeticion(...msg) {
	traza('procesaPeticion','msg',[msg])
	backend.send(msg)
}
function procesaRespuesta(...msg) {
	traza('procesaRespuesta','msg',[msg])
	frontend.send(msg)
}

frontend.on('message', procesaPeticion)
frontend.on('error'  , (msg) => {error(`${msg}`)})
 backend.on('message', procesaRespuesta)
 backend.on('error'  , (msg) => {error(`${msg}`)})
 process.on('SIGINT' , adios([frontend, backend],"abortado con CTRL-C"))
