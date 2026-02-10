const net = require('net')

const PROXY_IP = "127.0.0.1"
const PROXY_PORT = 8101

const client = net.connect({ host: PROXY_IP, port: PROXY_PORT }, function () {
    var msg = JSON.stringify({'remote_ip': 'sepie.es', 'remote_port': 80})
    client.write(msg)
    console.log('Sent \"' + msg + '\" to proxy')
    process.exit(0)
})