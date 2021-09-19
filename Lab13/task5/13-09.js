const dgram = require("dgram");

const hostname = "127.0.0.1";
const port = 5000;

const server = dgram.createSocket("udp4");

server.on("message", (msg, remoteInfo) => {
    console.log(`Received message from client ${remoteInfo.address}:${remoteInfo.port} -- ${msg}`);
    server.send(`ECHO: ${msg}`, remoteInfo.port, remoteInfo.address, (error) => {
        if (error) {
            console.error(`Error from client ${remoteInfo.address}:${remoteInfo.port} -- ${error.name}`, error.message);
        }
    })
});

server.bind(port, hostname, () => {
    console.log("UDP-Server started on -- " + hostname + ":" + port);
});