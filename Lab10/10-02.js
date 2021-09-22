const WebSocket = require("ws");

const webSocket = new WebSocket("ws://localhost:4000/ws-server");

webSocket.on("open", () =>
{
    console.log(`WebSocket: connected to ws-server`);
    let n = 1;
    const sendingInterval = setInterval(() => {
        webSocket.send(`10-01-client: ${n++}`);
    }, 3000);

    setTimeout(()=>
    {
        clearInterval(sendingInterval);
        webSocket.close();
    }, 25000);
})

webSocket.on("message", (msg) =>
{
    console.log(`WebSocket, new message: ${msg}`);
})

webSocket.on("error", (error) =>
{
    console.log(`WebSocket: error disappeared`);
    console.log(`error: ${error.message}`);
});

webSocket.on("close", () => {
    console.log(`WebSocket:  disconnected from ws-server`);
});