var fs = require('fs')
file = process.argv[2]

function sortLines(str) {
    return str.toString().split('\n').sort().join("\n")
}

var r = fs.readFileSync(file, 'utf-8')
fs.writeFileSync(file + '2', sortLines(r), 'utf-8')