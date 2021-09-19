const net = require("net");

const hostname = "127.0.0.1";
const port = 5000;
const server = net.createServer();

server.listen(port, hostname, () => {
    console.log(`TCP-server has started listening ${hostname}:${port}`)
})

server.on("connection", (clientSocket) => {
    console.log("Client connected: " + clientSocket.remoteAddress + ":" + clientSocket.remotePort);

    clientSocket.on("data", (data) => {
        console.log(`Received data from client -- ${data}`);
        clientSocket.write(`ECHO: ${data}`);
    })

    clientSocket.on("close", () => {
        console.log("Client disconnected: " + clientSocket.remoteAddress + ":" + clientSocket.remotePort);
    })
})