const fs = require('fs');

const fs = require('fs');
(new Promise((resolve, reject) => {
    fs.readFile('/etc/hosts', 'utf8', function (err, data) {
        if (err) {
            reject(err);
        }
        resolve(data);
})})).then(console.log, console.error)

async function readFile() {
    try {
        console.log(await readFilePromise)
    }
}