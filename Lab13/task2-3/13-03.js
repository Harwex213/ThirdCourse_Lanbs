const net = require("net");

const hostname = "127.0.0.1";
const port = 5000;
const server = net.createServer();

server.listen(port, hostname, () => {
    console.log(`TCP-server has started listening ${hostname}:${port}`)
});

server.on("connection", (clientSocket) => {
    const buf = Buffer.alloc(4);
    let sum = 0;
    console.log("Client connected: " + clientSocket.remoteAddress + ":" + clientSocket.remotePort);

    const intervalId = setInterval(() => {
        buf.writeInt32LE(sum, 0);
        clientSocket.write(buf);
        console.log(`Send sum to client #${clientSocket.remotePort} -- ${buf.readInt32LE()}`);
    }, 5000)

    clientSocket.on("data", (data) => {
        sum += data.readInt32LE();
        console.log(`Received number from client #${clientSocket.remotePort} -- ${data.readInt32LE()}`);
    })

    clientSocket.on("close", () => {
        clearInterval(intervalId);
        console.log("Client disconnected: " + clientSocket.remoteAddress + ":" + clientSocket.remotePort);
    })

    clientSocket.on("error", (err) => {
        console.log(`Error from client #${clientSocket.remotePort} -- ${err.name}`, err.message);
    })
})