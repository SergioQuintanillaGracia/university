const net = require("net");
const LOCAL_PORT = 8100;
const LOCAL_IP = "127.0.0.1";

if (process.argv.length != 4) {
    console.error("Please, provide the remote IP as the first argument, and the remote port as the second one.")
    process.exit(1)
}

const REMOTE_IP = process.argv[2];
const REMOTE_PORT = process.argv[3];

let proxyServer = null

function startProxy(ip, port) {
    if (proxyServer) {
        proxyServer.close(() => {
            console.log('Restarting proxy server')
        })
    }

    proxyServer = net.createServer(function (socket) {
        const serviceSocket = new net.Socket();
        serviceSocket.connect(parseInt(port), ip, function () {
            socket.on("data", function (msg) {
                console.log(msg + "");
                serviceSocket.write(msg);
            });
            serviceSocket.on("data", function (data) {
                socket.write(data);
            });
        });

    }).listen(LOCAL_PORT, LOCAL_IP);
}

const programmerReceiver = net.createServer(function (socket) {
    socket.on('data', function (data) {
        data = JSON.parse(data)
        let newIp = data['remote_ip']
        let newPort = data['remote_port']
        startProxy(newIp, newPort)
    })
}).listen(8101, '127.0.0.1')

startProxy(REMOTE_IP, REMOTE_PORT)
console.log("TCP server accepting connection on port: " + LOCAL_PORT);
