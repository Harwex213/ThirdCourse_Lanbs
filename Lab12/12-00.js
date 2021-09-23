const http = require("http");
const RPCWebSocket = require("rpc-websockets").Server;
const fs = require("fs");
const { dataDestination } = require("./const");
const errorResponse = require("./errorResponse");
const mainController = require("./controllers/mainController");
const backupController = require("./controllers/backupController");

const getRequestUrl = (request) => {
    const baseURL = "http" + "://" + request.headers.host + "/";
    return new URL(request.url, baseURL);
}

const getRoutePaths = (requestUrl) => {
    return requestUrl.pathname.match(/\/[A-Z-.]*/gi);
}

const getController = (routePaths) => {
    return routes[routePaths ? routePaths[0] : null];
}

const routes = {
    "/": mainController,
    "/backup": backupController,
}

http.createServer((request, response) => {
    try {
        const requestUrl = getRequestUrl(request);
        const routePaths = getRoutePaths(requestUrl);
        const controller = getController(routePaths);
        console.log(routePaths);
        if (controller) {
            const method = controller[request.method];

            if (method) {
                method({
                    request,
                    response,
                    requestUrl,
                    routePaths
                });
            }
            else {
                errorResponse(response, "Endpoint doesn't support such http method");
            }
        }
        else {
            errorResponse(response, {
                message: "Api doesn't have such path"
            });
        }
    } catch (error) {
        errorResponse(response, error);
    }
}).listen(4000, "localhost", () => console.log("We started"));

const socket = new RPCWebSocket({
    port: 5050,
    host: "localhost",
    path: "/"
});
socket.event("changed");

fs.watch(dataDestination, "utf-8", (eventType) => {
    if (eventType === "change") {
        socket.emit("changed");
    }
})