const {zmq, lineaOrdenes, traza, error, adios, creaPuntoConexion} = require('../tsr')
const ans_interval = 2000 // deadline to detect worker failure
lineaOrdenes("frontendPort backendPort")

let failed   = {}	// Map(worker:bool) failed workers has an entry
let working  = {}	// Map(worker:timeout) timeouts for workers executing tasks
let ready    = []	// List(worker) ready workers (for load-balance)
let pending  = []	// List([client,message]) requests waiting for workers
let frontend = zmq.socket('router')
let backend  = zmq.socket('router')

function dispatch(client, message) {
	traza('dispatch','client message',[client,message])
	if (ready.length) new_task(ready.shift(), client, message)
	else 			  pending.push([client,message])
}
function new_task(worker, client, message) {
	traza('new_task','client message',[client,message])
	working[worker] = setTimeout(()=>{failure(worker,client,message)}, ans_interval)
	backend.send([worker,'', client,'', message])
}
function failure(worker, client, message) {
	traza('failure','client message',[client,message])
	failed[worker] = true
	dispatch(client, message)
}
function frontend_message(client, sep, message) {
	traza('frontend_message','client sep message',[client,sep,message])
	dispatch(client, message)
}
function backend_message(worker, sep1, client, sep2, message) {
	traza('backend_message','worker sep1 client sep2 message',[worker,sep1,client,sep2,message])
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
