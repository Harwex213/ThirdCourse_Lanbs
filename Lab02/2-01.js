const http = require("http");
const fs = require("fs");

http.createServer((request, response) => {
    const makeResponse = (contentType, fileToRead) => {
        response.writeHead(200, {"Content-type": contentType});
        response.write(fs.readFileSync(fileToRead));
        response.end();
    }

    switch (request.url) {
        case "/html":
            makeResponse("text/html", "index.html");
            break;
        case "/png":
            makeResponse("image/png", "image.png");
            break;
        case "/api/name":
            makeResponse("text/plain; charset=UTF-8", "text.txt");
            break;
        case "/xmlhttprequest":
            makeResponse("text/html", "xmlhttprequest.html");
            break;
        case "/fetch":
            makeResponse("text/html", "xmlhttprequest.html");
            break;
        case "/jquery":
            makeResponse("text/html", "xmlhttprequest.html");
            break;
    }
}).listen(4000);

console.log("We started");