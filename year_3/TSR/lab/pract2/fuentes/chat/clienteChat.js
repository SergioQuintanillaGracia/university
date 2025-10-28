const {zmq, lineaOrdenes, error, adios, conecta} = require('../tsr')
lineaOrdenes("nick hostServidor portDifusion portPipeline")

let entrada = zmq.socket('sub')
let salida  = zmq.socket('push')
conecta(salida, hostServidor, portPipeline)
conecta(entrada,hostServidor, portDifusion)
entrada.subscribe('')
entrada.on('message', (nick,m) => {console.log('['+nick+'] '+m)})

process.stdin.resume()
process.stdin.setEncoding('utf8')
process.stdin.on('data' ,(str)=> {salida.send([nick, str.slice(0,-1)])})
process.stdin.on('end',()=> {salida.send([nick, 'BYE']); adios([entrada,salida],"Adios")()}) 

salida.on('error', (msg) => {error(`${msg}`)})
process.on('SIGINT', adios([entrada,salida],"abortado con CTRL-C"))

salida.send([nick,'HI'])
