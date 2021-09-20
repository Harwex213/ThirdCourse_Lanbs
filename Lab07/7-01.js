const http = require("http");
const fs = require("fs");
const fileManagerController = require("./m07-01/m07-01")("./static");

const sendError405 = (response) => {
    response.writeHead(405);
    response.end("Method GET only accepted");
}

const sendIndex = (response) => {
    response.writeHead(200, {
        "Content-type": "text/html"
    });
    response.write(fs.readFileSync("index.html"));
    response.end();
}

const routes = {
    "/": sendIndex,
}

const server = http.createServer((request, response) => {
    const baseURL =  request.protocol + '://' + request.headers.host + '/';
    const requestUrl = new URL(request.url, baseURL);

    if (request.method !== "GET") {
        sendError405(response);
        return;
    }

    const controllerFunc = routes[requestUrl.pathname] || fileManagerController.sendFile.bind(fileManagerController);
    controllerFunc(response, requestUrl.pathname);
});

server.listen(4000, () => console.log("We started"));