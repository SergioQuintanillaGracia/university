const { lineaOrdenes, traza, error, adios, creaPuntoConexion, conecta } = require('../../tsr')
zmq = require("zeromq/v5-compat")
lineaOrdenes("broker1Port backendPort")

let workers = [] // workers disponibles
let broker1 = zmq.socket('dealer')
let backend = zmq.socket('router')
conecta(broker1, "localhost", broker1Port)
creaPuntoConexion(backend, backendPort)

function procesaPeticion(cliente, msg) { // llega peticion desde cliente
	traza('procesaPeticion', 'cliente msg', [cliente, msg])
	if (workers.length) backend.send([workers.shift(), '', cliente, msg])
	else pendiente.push([cliente, msg])
}

function procesaMsgWorker(worker, sep, cliente, resp) {
	traza('procesaMsgWorker', 'worker sep cliente resp', [worker, sep, cliente, resp])

	// Add the worker to the list of available workers
	workers.push(worker)

	if (cliente) {  // A client was waiting for the answer
		broker1.send([cliente, resp])
	} else {
		// It's a worker connection message, notify broker1 that a new worker is available
		broker1.send(['', ''])
	}
}

broker1.on('message', procesaPeticion)
broker1.on('error', (msg) => { error(`${msg}`) })
backend.on('message', procesaMsgWorker)
backend.on('error', (msg) => { error(`${msg}`) })
process.on('SIGINT', adios([broker1, backend], "abortado con CTRL-C"))
