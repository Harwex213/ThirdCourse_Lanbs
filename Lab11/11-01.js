let WebSocket = require("ws");
const fs = require("fs");

const webServer = new WebSocket.Server({
    port: 4000,
    host: "localhost"
});

webServer.on("connection", (connection, request) =>
{
    const ip = request.socket.remoteAddress + ":" + request.socket.remotePort;
    console.log(`Just notice that we have a new connection here (${ip})`);

    const webSocketStream = WebSocket.createWebSocketStream(connection);
    const file = fs.createWriteStream(__dirname + `\\upload\\task-1-file.txt`, {
        encoding: "utf-8"
    });
    webSocketStream.pipe(file);
    console.log(`Upload file from connection ${ip} done successfully`);

    connection.on("close", () => console.log(`Just notice that connection ${ip} closed`));
});