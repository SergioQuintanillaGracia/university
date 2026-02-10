const { lineaOrdenes, traza, error, adios, creaPuntoConexion, conecta } = require('../../tsr')
zmq = require("zeromq/v5-compat")
lineaOrdenes("frontendPort broker2Port")

let pendiente = [] // peticiones no enviadas a ningun worker
let availableWorkers = 0
let frontend = zmq.socket('router')
let broker2 = zmq.socket('dealer')
creaPuntoConexion(frontend, frontendPort)
creaPuntoConexion(broker2, broker2Port)

function procesaPeticion(cliente, sep, msg) { // llega peticion desde cliente
	traza('procesaPeticion', 'cliente sep msg', [cliente, sep, msg])
	if (availableWorkers > 0) {
		broker2.send([cliente, msg])
		availableWorkers--
		console.log("# Currently available workers: " + availableWorkers)
	}
	else pendiente.push([cliente, msg])
}

function procesaMsgWorker(cliente, resp) {
	traza('procesaMsgWorker', 'worker sep cliente resp', [cliente, resp])

	// We know a worker is free
	// If there are pending tasks, just send one of them so it is processed
	if (pendiente.length) {
		let [c, m] = pendiente.shift()  // c = client, m = message
		broker2.send([c, m])
	} else {
		// There are no pending tasks, so add the worker as available
		availableWorkers++
		console.log("# Currently available workers: " + availableWorkers)
	}

	if (cliente) {
		frontend.send([cliente, '', resp])
	}
}

frontend.on('message', procesaPeticion)
frontend.on('error', (msg) => { error(`${msg}`) })
broker2.on('message', procesaMsgWorker)
broker2.on('error', (msg) => { error(`${msg}`) })
process.on('SIGINT', adios([frontend, broker2], "abortado con CTRL-C"))
