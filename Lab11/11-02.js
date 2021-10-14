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

    const webSocketStream = WebSocket.createWebSocketStream(connection, {
        encoding: "utf-8"
    });
    const file = fs.createReadStream("./download/task2.txt");
    file.pipe(webSocketStream);
    console.log(`Send file to connection ${ip} done successfully`);

    connection.on("close", () => console.log(`Just notice that connection ${ip} closed`));
});