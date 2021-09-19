const net = require("net");
const process = require("process");

const serverHostname = "127.0.0.1";
const serverPort = 5000;

const clientSocket = new net.Socket();

clientSocket.connect({
    port: serverPort,
    host: serverHostname,
}, () => {
    console.log("Connected to server: " + serverHostname + ":" + serverPort);
    const buf = Buffer.alloc(4);
    let number = Number(process.argv[2]);

    const intervalId = setInterval(() => {
        buf.writeInt32LE(number, 0);
        clientSocket.write(buf);
        console.log(`Send number to server -- ${buf.readInt32LE()}`);
    }, 1000);

    setTimeout(() => {
        clearInterval(intervalId);
        clientSocket.end();
    }, 20 * 1000)
})

clientSocket.on("data", (data => {
    console.log(`Received sum from server -- ${data.readInt32LE()}`);
}))

clientSocket.on("close", () => {
    console.log("Disconnected from server: " + serverHostname + ":" + serverPort);
});