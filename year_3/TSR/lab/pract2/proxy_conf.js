const net = require("net");
const LOCAL_PORT = 8100;
const LOCAL_IP = "127.0.0.1";

if (process.argv.length != 4) {
    console.error("Please, provide the remote IP as the first argument, and the remote port as the second one.")
    process.exit(1)
}

const REMOTE_IP = process.argv[2];
const REMOTE_PORT = process.argv[3];

const server = net.createServer(function (socket) {
    const serviceSocket = new net.Socket();
    serviceSocket.connect(parseInt(REMOTE_PORT), REMOTE_IP, function () {
        socket.on("data", function (msg) {
            console.log(msg + "");
            serviceSocket.write(msg);
        });
        serviceSocket.on("data", function (data) {
            socket.write(data);
        });
    });
}).listen(LOCAL_PORT, LOCAL_IP);

console.log("TCP server accepting connection on port: " + LOCAL_PORT);
