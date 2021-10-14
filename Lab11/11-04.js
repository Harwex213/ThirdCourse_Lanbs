let WebSocket = require("ws");

const server = new WebSocket.Server({
    port: 4000,
    host: "localhost"
});

let n = 0;
server.on("connection", (connection, request) =>
{
    const ip = request.socket.remoteAddress + ":" + request.socket.remotePort;
    console.log(`Just notice that we have a new connection here (${ip})`);

    connection.on("message", (message) => {
        const json = JSON.parse(message);
        json.server = n++;
        connection.send(JSON.stringify(json));
    })

    connection.on("close", () => console.log(`Just notice that connection ${ip} closed`));
});