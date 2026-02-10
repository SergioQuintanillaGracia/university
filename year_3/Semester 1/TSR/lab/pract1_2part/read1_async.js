const fs = require('fs');

async function readFile() {
    let promise = new Promise((resolve, reject) => {
        fs.readFile('/etc/hosts', 'utf8', function (err, data) {
            if (err) {
                reject(err);
            }
            resolve(data);
    })})

    const promiseResult = await promise
    console.log(promiseResult)
}

readFile()