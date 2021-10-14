const process = require("process");
const RpcWebsocketServer = require("rpc-websockets").Server;

const server = new RpcWebsocketServer({
    port: 4000,
    host: "localhost",
    path: "/"
});

server.event("A");
server.event("B");
server.event("C");

process.stdin.setEncoding("utf-8");
process.stdin.on("readable", () => {
    let chunk = null;
    while ((chunk = process.stdin.read()) != null) {
        const event = chunk.trim();
        if (event.match(/^[ABC]$/)) {
            server.emit(event);
            console.log("Was emitted event " + event);
        }
    }
})