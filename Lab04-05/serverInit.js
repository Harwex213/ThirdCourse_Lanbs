const http = require("http");
const database = require("./databaseInit");
const statistic = require("./statistic/statistic");
const fs = require("fs");

let serverInit = http.createServer((request, response) => {
    if (statistic.isGathering()) {
        statistic.onQuery();
    }

    const baseURL =  request.protocol + '://' + request.headers.host + '/';
    const requestUrl = new URL(request.url, baseURL);

    switch (requestUrl.pathname) {
        case "/api/db":
            switch (request.method)
            {
                case "GET":
                case "POST":
                case "PUT":
                case "DELETE":
                    database.emit(request.method, request, response, database);
                    break;
                default:
                    break;
            }
            break;
        case "/api/ss":
            response.writeHead(200, {'Content-Type': 'application/json; charset=utf-8'});
            response.end(JSON.stringify(statistic.getCurrentStatistic()));
            break;
        case "/":
            response.writeHead(200, {"Content-type": "text/html"});
            response.write(fs.readFileSync("index.html"));
            response.end();
            break;
    }
});

serverInit.listen(4000, () => console.log("We started"));

module.exports = serverInit;