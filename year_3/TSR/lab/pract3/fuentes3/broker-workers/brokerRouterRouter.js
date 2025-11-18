const {lineaOrdenes, traza, error, adios, creaPuntoConexion} = require('../tsr')
zmq = require("zeromq/v5-compat")
lineaOrdenes("frontendPort backendPort")
let workers  =[] // workers disponibles
let pendiente=[] // peticiones no enviadas a ningun worker
let totalPetitions = 0
let workersPetitions = new Map()
let frontend = zmq.socket('router')
let backend  = zmq.socket('router')
creaPuntoConexion(frontend, frontendPort)
creaPuntoConexion(backend,  backendPort)

function procesaPeticion(cliente,sep,msg) { // llega peticion desde cliente
	traza('procesaPeticion','cliente sep msg',[cliente,sep,msg])
	worker = workers.shift()
	totalPetitions++
	if (!workersPetitions.has(worker)) {
		workersPetitions.set(worker, 0)
	} else {
		workersPetitions.set(worker, workersPetitions.get(worker) + 1)
	}
	if (workers.length) backend.send([worker,'',cliente,msg])
	else pendiente.push([cliente,msg])
}
function procesaMsgWorker(worker,sep,cliente,resp) {
	traza('procesaMsgWorker','worker sep cliente resp',[worker,sep,cliente,resp])
	if (pendiente.length) { // hay trabajos pendientes. Le pasamos el mas antiguo al worker
		let [c,m] = pendiente.shift()
		backend.send([worker,'',c,m])
	}
	else workers.push(worker) // añadimos al worker como disponible
	if (cliente) frontend.send([cliente,'',resp]) // habia un cliente esperando esa respuesta
}

frontend.on('message', procesaPeticion)
frontend.on('error'  , (msg) => {error(`${msg}`)})
 backend.on('message', procesaMsgWorker)
 backend.on('error'  , (msg) => {error(`${msg}`)})
 process.on('SIGINT' , adios([frontend, backend],"abortado con CTRL-C"))
