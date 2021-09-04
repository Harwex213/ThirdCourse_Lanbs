const http = require("http");

http.createServer((request, response) => {
    response.writeHead(200, {"Content-type": "text/html"});
    response.end("<div>Hello World</div>")
}).listen(4000);

console.log("We started");