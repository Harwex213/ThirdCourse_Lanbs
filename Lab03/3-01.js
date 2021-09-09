const http = require("http");
const process = require("process");

let state = "norm";
const availableState = ["norm", "stop", "test", "idle", "exit"];

http.createServer((request, response) => {
    response.writeHead(200, {"Content-type": "text/html"});
    response.end(`<div>${state}</div>`)
}).listen(4000);

console.log("We started");

const setState = (newState, callback) => {
    if (availableState.includes(newState)) {
        console.log(`previous state: ${state} --> next state:  ${newState}`);
        state = newState;
        if (typeof callback === "function") {
            callback();
        }
    }
    else {
        console.log(`Undefined state: ${newState}`);
    }
}

process.stdin.setEncoding("utf-8");
process.stdin.on("readable", () => {
    let chunk = null;
    while ((chunk = process.stdin.read()) != null) {
        const trimmedChunk = chunk.trim();
        let callback = null;
        if (trimmedChunk === "exit") {
            callback = () => {
                process.exit(0);
            }
        }
        setState(trimmedChunk, callback);
    }
})