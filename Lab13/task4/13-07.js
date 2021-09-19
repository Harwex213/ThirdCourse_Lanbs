const net = require("net");

const hostname = "127.0.0.1";
const portOne = 40000;
const portSecond = 50000;

const handleConnection = (clientSocket) => {
    console.log("Client connected: " + clientSocket.remoteAddress + ":" + clientSocket.remotePort);

    clientSocket.on("data", (data) => {
        clientSocket.write(`ECHO: ${data.readInt32LE()}`);
        console.log(`Received number from client #${clientSocket.remotePort} -- ${data.readInt32LE()}`);
    })

    clientSocket.on("close", () => {
        console.log("Client disconnected: " + clientSocket.remoteAddress + ":" + clientSocket.remotePort);
    })

    clientSocket.on("error", (err) => {
        console.log(`Error from client #${clientSocket.remotePort} -- ${err.name}`, err.message);
    })
};

net.createServer(handleConnection).listen(portOne, hostname, () => {
    console.log(`TCP-server has started listening ${hostname}:${portOne}`);
})

net.createServer(handleConnection).listen(portSecond, hostname, () => {
    console.log(`TCP-server has started listening ${hostname}:${portSecond}`);
})