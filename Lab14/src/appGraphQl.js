// const graphQlController = require("./controllers/graphQlController");
//
// const parseRequestUrl = (request) => {
//     const baseURL = "http" + "://" + request.headers.host + "/";
//     return new URL(request.url, baseURL);
// }
//
// const parseBody = (request) => new Promise((resolve => {
//     let body = "";
//
//     request.on("data", (data) => body += data);
//     request.on("end", () => resolve(JSON.parse(body)));
// }))
//
// const postRoutes = {
//     "/graphql": graphQlController.handleGraphQlRequest
// }
//
// const methods = {
//     "POST": postRoutes,
// }
//
// const unhandledApi = (response) => {
//     response.writeHead(400, {
//         "Content-Type" : "text/plain; charset=utf-8"
//     })
//     response.end("Api does not have such endpoint");
// }
//
// const app = async (request, response) => {
//     const requestUrl = parseRequestUrl(request);
//     const routes = methods[request.method];
//     const routePaths = requestUrl.pathname.match(/\/[A-Z-.]+/gi);
//     const controllerMethod = routes[routePaths ? routePaths[0] : null];
//
//     if (controllerMethod) {
//         request.body = await parseBody(request);
//         controllerMethod(request, response);
//     }
//     else {
//         unhandledApi(response);
//     }
// }
//
// module.exports = app;