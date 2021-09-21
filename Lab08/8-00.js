const http = require("http");
const { parseRequestUrl } = require("./utils");
const connectionController = require("./api/8-01");
const headersController = require("./api/8-02");
const parameterController = require("./api/8-03");
const closeController = require("./api/8-05");
const socketController = require("./api/8-06");
const requestDataController = require("./api/8-07");
const responseStatusController = require("./api/8-08");
const formParamsController = require("./api/8-09");
const jsonController = require("./api/8-10");
const xmlController = require("./api/8-11");
const filesController = require("./api/8-12");
const uploadController = require("./api/8-14");

const getRoutes = {
    "/connection": connectionController.getConnection,
    "/headers": headersController.getHeaders,
    "/parameter": parameterController.getParameters,
    "/close": closeController.closeServer,
    "/socket": socketController.getSockets,
    "/req-data": requestDataController.analyzeRequest,
    "/resp-status": responseStatusController.getResponse,
    "/formparameter": formParamsController.getFormParams,
    "/files": filesController.getFiles,
    "/upload": uploadController.getUpload,
}

const postRoutes = {
    "/formparameter": formParamsController.postFormParams,
    "/json": jsonController.postJson,
    "/xml": xmlController.postXml,
    "/upload": uploadController.postUpload,
}

const methods = {
    "GET": getRoutes,
    "POST": postRoutes,
}

const unhandledApi = (response) => {
    response.writeHead(400, {
        "Content-Type" : "text/plain; charset=utf-8"
    })
    response.end("Api does not have such method or endpoint");
}

const server = http.createServer((request, response) => {
    const requestUrl = parseRequestUrl(request);
    const routes = methods[request.method];
    const routePaths = requestUrl.pathname.match(/\/[A-Z-.]+/gi);
    const controllerMethod = routes[routePaths ? routePaths[0] : null];

    if (controllerMethod) {
        controllerMethod(request, response, {
            requestUrl,
            server,
            routePaths,
            viewsDirectory: "./view",
            staticDirectory: "./static"
        });
    }
    else {
        unhandledApi(response);
    }
}).listen(4000, "localhost", () => console.log("We started"));

const connections = [];
server.on("connection", (socket) => {
    connections.push(socket);
})
server.on("startingClose", () => {
    connections.forEach(socket => {
        socket.destroy();
    })
})