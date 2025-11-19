const { lineaOrdenes, traza, error, adios, creaPuntoConexion } = require('../../tsr')
zmq = require("zeromq/v5-compat")
lineaOrdenes("broker1Port backendPort")
let workers = [] // workers disponibles
let pendiente = [] // peticiones no enviadas a ningun worker
let tobroker1 = zmq.socket('router')
let backend = zmq.socket('router')
creaPuntoConexion(tobroker1, broker1Port)
creaPuntoConexion(backend, backendPort)

let broker1Id = null

function sendAvailableWorkerMessage() {
	if (broker1Id) {
		tobroker1.send([broker1Id, 'available_worker'])
	}
}

function procesaPeticionBroker1(id, type, ...args) { // llega peticion desde cliente
	let msgType = type.toString()
	
	if (msgType == 'HELLO') {
		broker1Id = id

		for (let i = 0; i < workers.length; i++) {
			sendAvailableWorkerMessage()
		}
	} else if (msgType == 'request') {
		let [cliente, sep, msg] = args

		traza('procesaPeticion', 'cliente sep msg', [cliente, sep, msg])
		if (workers.length) backend.send([workers.shift(), '', cliente, '', msg])
		else pendiente.push([cliente, sep, msg])
	}
}

function procesaMsgWorker(worker, sep, ...args) {
	traza('procesaMsgWorker', 'worker args', [worker, args])

	let [cliente, sep2, resp] = args
	let isReply = (cliente && resp)

	if (pendiente.length) { // hay trabajos pendientes. Le pasamos el mas antiguo al worker
		let [c, sep2, m] = pendiente.shift()
		backend.send([worker, '', c, '', m])
	}
	else {
		workers.push(worker) // añadimos al worker como disponible
		sendAvailableWorkerMessage()
	}
	if (isReply) {
		if (broker1Id) tobroker1.send([broker1Id, 'reply', cliente, '', resp]) // habia un cliente esperando esa respuesta
		else error("Received reply but broker 1 is not connected yet")
	}
}

tobroker1.on('message', procesaPeticionBroker1)
tobroker1.on('error', (msg) => { error(`${msg}`) })
backend.on('message', procesaMsgWorker)
backend.on('error', (msg) => { error(`${msg}`) })
process.on('SIGINT', adios([tobroker1, backend], "abortado con CTRL-C"))
