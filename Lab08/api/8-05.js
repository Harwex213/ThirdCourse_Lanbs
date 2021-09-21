const closeServer = (request, response, args) => {
    const { server } = args;

    setTimeout(() => {
        server.emit("startingClose");
        server.close(() => console.log("Server close"));
    }, 10000)

    response.writeHead(200, {
        "Content-Type" : "text/plain; charset=utf-8"
    })
    response.end("Server will be close through 10 sec...")
};

module.exports = {
    closeServer
}