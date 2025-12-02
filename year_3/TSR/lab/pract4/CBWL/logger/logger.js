const {zmq, lineaOrdenes, traza, error, adios, creaPuntoConexion} = require('../tsr')
lineaOrdenes("loggerPort filename")
// logger in NodeJS
// First argument is port number for incoming messages
// Second argument is file path for appending log entries

const fs = require('fs');
let log = zmq.socket('pull')

creaPuntoConexion(log, loggerPort)
log.on('message', (text) => {fs.appendFileSync(filename, text+'\n')})
