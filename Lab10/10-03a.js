const WebSocket = require("ws");
const process = require("process");

const webSocket = new WebSocket("ws://localhost:4000/ws-broadcast-server");

webSocket.on("open", () =>
{
    console.log(`Connected to ws-server`);
})

webSocket.on("message", (msg) =>
{
    console.log(`New message: ${msg}`);
})

webSocket.on("error", (error) =>
{
    console.log(`Error disappeared`);
    console.log(`error: ${error.message}`);
});

webSocket.on("close", () => {
    console.log(`Disconnected from ws-server`);
});

process.stdin.setEncoding("utf-8");
process.stdin.on("readable", () => {
    let chunk = null;
    while ((chunk = process.stdin.read()) != null) {
        const trimmedChunk = chunk.trim();
        if (trimmedChunk === "exit") {
            webSocket.close();
            return;
        }
        webSocket.send(trimmedChunk);
    }
})