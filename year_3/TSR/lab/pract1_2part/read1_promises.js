const fs = require('fs');
(new Promise((resolve, reject) => {
    fs.readFile('/etc/hosts', 'utf8', function (err, data) {
        if (err) {
            reject(err);
        }
        resolve(data);
})})).then(console.log, console.error)