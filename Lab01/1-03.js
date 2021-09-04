const http = require("http");

http.createServer((request, response) => {
    response.writeHead(200, {"Content-type": "text/html"});

    let headersString = "";
    for(const key in request.headers)
    {
        headersString += "<p>" + key + ": " + request.headers[key];
    }

    response.end("<p><b>Method: </b>" + request.method +
        "<p><b>Http Version: </b>" + request.httpVersion +
        "<p><b>Url: </b>" + request.url +
        "<p><b>Headers: </b>" + headersString);
}).listen(4000);

console.log("We started");