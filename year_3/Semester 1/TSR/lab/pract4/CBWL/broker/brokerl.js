const {zmq, lineaOrdenes, traza, error, adios, creaPuntoConexion, conecta} = require('../tsr')
const ans_interval = 2000 // deadline to detect worker failure
lineaOrdenes("frontendPort backendPort loggerHost loggerPort")

let failed   = {}	// Map(worker:bool) failed workers has an entry
let working  = {}	// Map(worker:timeout) timeouts for workers executing tasks
let ready    = []	// List(worker) ready workers (for load-balance)
let pending  = []	// List([client,message]) requests waiting for workers
let frontend = zmq.socket('router')
let backend  = zmq.socket('router')
let slogger  = zmq.socket('push')

slogger.send("broker starts")

function dispatch(client, message) {
	traza(slogger, 'dispatch','client message',[client,message])
	if (ready.length) new_task(ready.shift(), client, message)
	else 			  pending.push([client,message])
}
function new_task(worker, client, message) {
	traza(slogger, 'new_task','client message',[client,message])
	working[worker] = setTimeout(()=>{failure(worker,client,message)}, ans_interval)
	backend.send([worker,'', client,message])
}
function failure(worker, client, message) {
	traza(slogger, 'failure','client message',[client,message])
	failed[worker] = true
	dispatch(client, message)
}
function frontend_message(client, sep, message) {
	traza(slogger, 'frontend_message','client sep message',[client,sep,message])
	dispatch(client, message)
}
function backend_message(worker, sep, client, message) {
	traza(slogger, 'backend_message','worker sep client message',[worker,sep,client,message])
	if (failed[worker]) return  // ignore messages from failed nodes
	if (worker in working) { // task response in-time
		clearTimeout(working[worker]) // cancel timeout
		delete(working[worker])
	}
	if (pending.length) new_task(worker, ...pending.shift())
	else ready.push(worker)
	if (client) frontend.send([client,'',message])
}

frontend.on('message', frontend_message)
 backend.on('message', backend_message)
frontend.on('error'  , (msg) => {error(`${msg}`)})
 backend.on('error'  , (msg) => {error(`${msg}`)})
 process.on('SIGINT' , adios([frontend, backend],"abortado con CTRL-C"))
 
creaPuntoConexion(frontend, frontendPort)
creaPuntoConexion( backend,  backendPort)
conecta(slogger, loggerHost, loggerPort)
