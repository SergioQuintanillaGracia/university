var fs = require('fs')
file = process.argv[2]

function sortLines(str) {
    return str.toString().split('\n').sort().join("\n")
}

var r = fs.readFileSync(file, 'utf-8')
// fs.writeFileSync(file + '2', sortLines(r), 'utf-8')
fs.writeFile(file + '2', sortLines(r), 'utf-8', (err) => {
    if (err) {
        console.error('An error happened: ' + err)
    } else {
        console.log('End')
    }
})