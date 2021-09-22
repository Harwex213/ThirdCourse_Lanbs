const webSocket = require("ws");

const wsServer = new webSocket.Server({
    host: "localhost",
    port: 4000,
    path: "/ws-broadcast-server"
}, () => console.log("WebSocket server started"));

wsServer.on("connection", (connection, request) =>
{
    const id = `${request.socket.remoteAddress}:${request.socket.remotePort}`;
    console.log(`Connected ${id}`);

    connection.on("message", (message) =>
    {
        for (const client of wsServer.clients) {
            if (client.readyState !== webSocket.OPEN) {
                continue;
            }
            if (client === connection) {
                continue;
            }
            client.send(message);
        }
        console.log(`webSocket Server, message from client ${id}: ${message}`);
    });
    connection.on("close", () => {
        console.log(`webSocket Server: client with id ${id} disconnected`)
    });
});