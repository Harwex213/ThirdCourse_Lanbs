const net = require("net");

console.log(process.argv);

const serverHostname = "127.0.0.1";
const serverPort = 5000;

const clientSocket = new net.Socket();

clientSocket.connect({
    port: serverPort,
    host: serverHostname,
}, () => {
    console.log("Connected to server: " + serverHostname + ":" + serverPort);

    const intervalId = setInterval(() => {
        const data = "Hello from client";
        console.log(`Send data to server -- ${data}`);
        clientSocket.write(data);
    }, 1000);

    setTimeout(() => {
        clearInterval(intervalId);
        clientSocket.end();
    }, 10000)
})

clientSocket.on("data", (data => {
    console.log(`Received data from server -- ${data}`);
}))

clientSocket.on("close", () => {
    console.log("Disconnected from server: " + serverHostname + ":" + serverPort);
});