const net = require('net')
const fs = require('fs')

function getLoad() {
    data = fs.readFileSync("/proc/loadavg");
    var tokens = data.toString().split(' ');
    var min1 = parseFloat(tokens[0]) + 0.01;
    var min5 = parseFloat(tokens[1]) + 0.01;
    var min15 = parseFloat(tokens[2]) + 0.01;
    return min1 * 10 + min5 * 2 + min15;
};

const server = net.createServer(function (c) { // connection listener
    console.log('server: client connected');
    c.on('end', function () {
        console.log('server: client disconnected');
    });

    let serverIp = c.localAddress

    c.on('data', function (data) {
        data = JSON.parse(data)
        if (data.query === 'getLoad') {
            c.write(JSON.stringify({ serverIp: serverIp, load: getLoad() }));
        }
        c.end(); // close socket
    });
});

server.listen(8000, function () { // listening listener
    console.log('server bound');
});
