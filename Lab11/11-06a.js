const RpcWebsocketClient = require("rpc-websockets").Client;

const client = new RpcWebsocketClient("ws://localhost:4000");

const event = process.argv[2];

client.on("open", async () => {
    await client.subscribe(event);

    client.on(event, () => console.log("Event was emitted from server: " + event));
})