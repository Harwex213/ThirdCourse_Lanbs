const RpcWebsocketClient = require("rpc-websockets").Client;
const process = require("process");

const client = new RpcWebsocketClient("ws://localhost:4000");

client.on("open", async () => {
    process.stdin.setEncoding("utf-8");
    process.stdin.on("readable", () => {
        let chunk = null;
        while ((chunk = process.stdin.read()) != null) {
            const event = chunk.trim();
            if (event.match(/^[ABC]$/)) {
                client.notify(event);
                console.log("Was notified event " + event);
            }
        }
    })
})