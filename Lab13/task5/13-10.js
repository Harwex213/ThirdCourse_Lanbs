const dgram = require("dgram");

const message = "Hello from client";
const client = dgram.createSocket("udp4");

const closeClient = () => {
    client.close();
    clearInterval(intervalId);
}

const sendMessage = () => client.send(message, 5000, "127.0.0.1", error => {
    if (error) {
        console.error(`${error.name}, ${error.message}`);
        closeClient();
    }
});

const intervalId = setInterval(() => sendMessage(), 1000);
setTimeout(() => {
    closeClient();
}, 10000)

client.on("message", (msg, _) => {
    console.log(`Received message from server -- ${msg}`);
})