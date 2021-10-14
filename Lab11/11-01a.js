const WebSocket = require("ws");
const fs = require("fs");

const webSocket = new WebSocket("ws://localhost:4000/");
const webSocketStream = WebSocket.createWebSocketStream(webSocket);
const file = fs.createReadStream("./task2.txt", {
    encoding: "utf-8"
});

file.pipe(webSocketStream);