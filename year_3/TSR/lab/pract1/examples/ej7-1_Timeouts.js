'use strict'

console.log ("uno")

setTimeout ( () => {
	console.log ("dos")
}, 100)

// Hacer trabajo
for (let i=0; i<100; i++) {}

setTimeout ( () => {
	console.log ("tres")
}, 99)

setTimeout ( () => {
	console.log ("cuatro")
}, 0)

console.log ("cinco")
