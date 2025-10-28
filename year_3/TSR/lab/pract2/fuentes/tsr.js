// ------------------------------- tsr.js
const zmq  = require('zeromq')
const path = require('node:path')

function error(msg) {
	console.error(`\x1b[31m ${msg} \x1b[0m `)
	process.exit(1)
}

function lineaOrdenes(params) {	//Comprobacion de parametros en linea de órdenes. Crea variables asociadas
	var args = params.split(" "), prog = path.win32.basename(process.argv[1])
	if( process.argv.length != (args.length+2) ) 
		error(`Parámetros incorrectos. Uso: node ${prog} ${params}`)
	args.forEach((param,i) => {global[param] = process.argv[2+i]})
	console.log(`Arranca el programa ${prog} con los siguientes parámetros en línea de órdenes:`)
	for (let a in args) {console.log(`\t${args[a]}\t|${global[args[a]]}|`)}
}

function traza(f,names,value) { //muestra los argumentos al invocar la función f
	console.log(`funcion ${f}`)
	var args = names.split(" ")
	for (let a in args) console.log(`\t${args[a]}\t|${value[a]}|`)
}

function adios(sockets, despedida) {		//cierra la conexión y el proceso
	return ()=>{
		console.log(`\x1b[33m ${despedida} \x1b[0m`)
		for (let s in sockets) sockets[s].close()
		process.exit(0)
	}
}

function creaPuntoConexion(socket,port) {
	console.log(`Creando punto conexión en port ${port} ...`)
	socket.bind(`tcp://*:${port}`, (err)=>{
		if (err) error(err)
		else console.log(`Escuchando en el port ${port}`)
	})
}

function conecta(socket,ip,port) {
	console.log(`Conectando con ip ${ip} port ${port}`)
	socket.connect(`tcp://${ip}:${port}`)
}

module.exports = {zmq, error, lineaOrdenes, traza, adios, creaPuntoConexion, conecta}
