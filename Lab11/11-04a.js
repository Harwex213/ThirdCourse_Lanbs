const WebSocket = require("ws");
const process = require("process");

const webSocket = new WebSocket("ws://localhost:4000/");

webSocket.on("open", () =>
{
    console.log(`Connected to ws-server`);
    webSocket.send(JSON.stringify({
        client: process.argv[2],
        timestamp: new Date().toISOString()
    }))
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