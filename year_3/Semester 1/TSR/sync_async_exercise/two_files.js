const fs = require('fs')

if (process.argv.length < 4) {
    console.log("Please, provide 2 file names")
    process.exit(1)
}

let file1 = process.argv[2]
let file2 = process.argv[3]

{
    // #####  -  EL CLEANEST CODE EVER  -  #####
    let counter = 0
    function checkEnd() {
        if (++counter == 2)
            console.log("End all!")
    }
}

processFile(file1)
processFile(file2)

function sortLines(str) {
    return str.toString().split('\n').sort().join("\n")
}

function processFile(file) {
    fs.readFile(file, 'utf-8', (error, data) => {
        if (error) {
            console.error("Error reading file")
            checkEnd()
        } else {
            fs.writeFile(file + '2', sortLines(data), 'utf-8', (err) => {
                if (err) {
                    console.error('An error happened: ' + err)
                } else {
                    console.log('End', file)
                }
            })

            checkEnd()
        }
    })
}
