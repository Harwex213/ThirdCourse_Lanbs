const getSockets = (request, response) => {
    response.writeHead(200, {
        "Content-Type": "text/html; charset=utf-8"
    });
    response.write(`<h3>ServerAddress =  ${request.socket.localAddress}</h3>`);
    response.write(`<h3>ServerPort = ${request.socket.localPort}</h3>`);
    response.write(`<h3>ClientAddress = ${request.socket.remoteAddress}</h3>`);
    response.write(`<h3>ClientPort = ${request.socket.remotePort}</h3>`);
    response.end();
};

module.exports = {
    getSockets
}