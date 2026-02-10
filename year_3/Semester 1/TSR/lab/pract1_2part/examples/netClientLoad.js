const net = require('net');

let serverIp = process.argv[2]
let ip = process.argv[3]

const client = net.connect({ host: serverIp, port: 8000 }, function () { // connect listener
    console.log('client connected');
    client.write(JSON.stringify({ clientIp: ip, query: 'getLoad' }));
});

client.on('data', function (data) {
    data = JSON.parse(data)
    console.log('Server load: ' + data.load);
    client.end(); // no more data written to the stream
});

client.on('end', function () {
    console.log('client disconnected');
});
