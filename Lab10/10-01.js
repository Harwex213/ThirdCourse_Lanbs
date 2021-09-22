const http = require("http");
const fs = require("fs");
const ws = require("ws");

http.createServer((request, response) => {
    if (request.method === "GET" && request.url === "/start") {
        response.writeHead(200, {
            "Content-Type": "text/html; utf-8"
        })
        const file = fs.readFileSync("index.html");
        response.end(file);
    } else {
        response.writeHead(400);
        response.end("Bad request");
    }
}).listen(3000, "localhost", () => console.log("HTTP server started"));

const wsServer = new ws.Server({
    host: "localhost",
    port: 4000,
    path: "/ws-server"
}, () => console.log("WebSocket server started"));

let globalId = 0;
wsServer.on("connection", (clientWs) =>
{
    const id = globalId++;
    console.log(`webSocket Server: client connected with id ${id}`);
    let k = 1;
    let n = 0;

    const intervalId = setInterval(() =>
    {
        clientWs.send(`10-01-server: ${n}->${k++}`)
    }, 5000);

    clientWs.on("message", (msg) =>
    {
        console.log(`webSocket Server, message from client ${id}: ${msg}`);
        n = parseInt(msg.toString()[msg.length - 1]);
    });
    clientWs.on("close", () => {
        clearInterval(intervalId);
        console.log(`webSocket Server: client with id ${id} disconnected`)
    });
});