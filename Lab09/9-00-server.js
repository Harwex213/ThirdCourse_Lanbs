const http = require("http");
const api = require("./api");

const getRoutes = {
    "/task-1": api.handleTask1,
    "/task-2": api.handleTask2,
    "/task-8": api.handleTask8,
}

const postRoutes = {
    "/task-3": api.handleTask3,
    "/task-4": api.handleTask4,
    "/task-5": api.handleTask5,
    "/task-6": api.handleTask6And7,
    "/task-7": api.handleTask6And7,
}

const methods = {
    "GET": getRoutes,
    "POST": postRoutes,
}

http.createServer((request, response) => {
    const baseURL = "http" + "://" + request.headers.host + "/";
    const requestUrl = new URL(request.url, baseURL);
    const routes = methods[request.method];
    const routePaths = requestUrl.pathname.match(/\/[A-Z-.0-9]+/gi);
    console.log(routePaths);
    const controllerMethod = routes[routePaths ? routePaths[0] : null];

    if (controllerMethod) {
        controllerMethod(request, response);
    }
    else {
        response.end("You pick the wrong side, bro");
    }
}).listen(4000, "localhost", () => console.log("We started"));
