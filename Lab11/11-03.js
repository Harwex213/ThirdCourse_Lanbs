let WebSocket = require("ws");

let n = 0;
const server = new WebSocket.Server({
    port: 4000,
    host: "localhost"
});

server.on("connection", (connection, request) =>
{
    const ip = request.socket.remoteAddress + ":" + request.socket.remotePort;
    console.log(`Just notice that we have a new connection here (${ip})`);
    connection.on("close", () => console.log(`Just notice that connection ${ip} closed`));
});

setInterval(() => {
    console.log("Starting ping to all clients");
    server.clients.forEach((client) => client.ping("mega ping"));
    console.log(`Ping done. Connections count: ${server.clients.size}`);
}, 5000);

setInterval(() => {
    console.log("Starting send messages to all clients");
    server.clients.forEach((client) => client.send(`11-03-server: ${n++}`));
}, 15000);