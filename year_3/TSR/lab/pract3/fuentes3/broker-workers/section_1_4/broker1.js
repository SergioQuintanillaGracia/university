const { lineaOrdenes, traza, error, adios, creaPuntoConexion, conecta } = require('../../tsr')
zmq = require("zeromq/v5-compat")
lineaOrdenes("frontendPort broker2Port")
let pendiente = [] // peticiones no enviadas a ningun worker
let availableWorkers = 0
let frontend = zmq.socket('router')
let tobroker2 = zmq.socket('dealer')
creaPuntoConexion(frontend, frontendPort)
conecta(tobroker2, "localhost", broker2Port)

// Send an initial message so we can get the number of available workers
// when we connect to the broker
tobroker2.send(['HELLO'])

function procesaPeticion(cliente, sep, msg) { // llega peticion desde cliente
	traza('procesaPeticion', 'cliente sep msg', [cliente, sep, msg])
	if (availableWorkers > 0) {
		tobroker2.send(['request', cliente, '', msg])
		availableWorkers--
	}
	else pendiente.push([cliente, '', msg])
}

function procesaMsgBroker2(type, ...args) {
	traza('procesaMsgBroker2', 'type args', [type, args])
	let msgType = type.toString()

	if (msgType == 'available_worker') {
		if (pendiente.length) { // hay trabajos pendientes. Le pasamos el mas antiguo al worker
			let [client, sep, msg] = pendiente.shift()
			tobroker2.send(['request', client, '', msg])
		} else {
			availableWorkers++
		}
	} else if (msgType == 'reply') {
		let [cliente, sep, resp] = args
		if (cliente) {
			frontend.send([cliente, '', resp]) // habia un cliente esperando esa respuesta
		}
	}
}

frontend.on('message', procesaPeticion)
frontend.on('error', (msg) => { error(`${msg}`) })
tobroker2.on('message', procesaMsgBroker2)
tobroker2.on('error', (msg) => { error(`${msg}`) })
process.on('SIGINT', adios([frontend, tobroker2], "abortado con CTRL-C"))
