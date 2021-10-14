const RpcWebsocketServer = require("rpc-websockets").Server;

const server = new RpcWebsocketServer({
    port: 4000,
    host: "localhost",
    path: "/"
});

server.register("A", () => console.log("Was notified event A"));
server.register("B", () => console.log("Was notified event B"));
server.register("C", () => console.log("Was notified event C"));