const eve = require('events')
const ev = new eve.EventEmitter
const e1 = 'one'
const e2 = 'two'
const e3 = 'three'

{
    let length = 2000

    function getLength() {
        return length
    }

    function triplicateLength() {
        length = length * 3 <= 18000 ? length * 3 : 18000
    }
}

{
    // Counter for the events
    let ce1 = 0;
    let ce2 = 0;

    function getListener(event) {
        return function() {
            if (event === e1) {
                ce1++
                console.log("Event", event)
            } else if (event === e2) {
                ce2++
                if (ce2 > ce1) console.log("Event", event)
                else console.log("There are more e1 events")
                
                setTimeout(() => {
                    ev.emit(event)
                }, getLength())
            } else if (event === e3) {
                console.log("Event", event)
                triplicateLength()
            }
        }
    }
}

ev.on(e1, getListener(e1))
ev.on(e2, getListener(e2))
ev.on(e3, getListener(e3))

setInterval(() => ev.emit(e1), 3000)
setTimeout(() => ev.emit(e2), getLength())
setInterval(() => ev.emit(e3), 10000)