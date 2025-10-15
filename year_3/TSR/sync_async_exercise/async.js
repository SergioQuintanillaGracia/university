var fs = require('fs')
file = process.argv[2]

function sortLines(str) {
    return str.toString().split('\n').sort().join("\n")
}

// var r = fs.readFileSync(file, 'utf-8')
fs.readFile(file, 'utf-8', (error, data) => {
    if (error) {
        console.error("Error reading file")
    } else {
        fs.writeFile(file + '2', sortLines(data), 'utf-8', (err) => {
            if (err) {
                console.error('An error happened: ' + err)
            } else {
                console.log('End')
            }
        })
    }
})
